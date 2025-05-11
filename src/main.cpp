#include <MFRC522.h>
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>

#define LILYGO_T_A7670
#define TINY_GSM_RX_BUFFER 1024
#include "utilities.h"
#include <FareMatrix.h>
#include <TinyGsmClient.h>
#include <TinyGPS++.h>
#include <vector>
#define RST_PIN 4
#define SS_PIN 5
#define BUZZER_PIN 15
#define LED_BLUE 12
#define LED_GREEN 13
#define LED_RED 14

#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon
#define TAP_WINDOW 10000 // 10 seconds, adjust as needed

MFRC522 rfid(SS_PIN, RST_PIN);
DS3231 rtc(SDA, SCL);
Time t;

unsigned long currentTime;
unsigned long lastTapTime = 0;  // Last time a card was tapped  // Current time
unsigned long tapThreshold = 5000;  // 2-second threshold in milliseconds
unsigned long gpsRefreshTime = 0;
unsigned long lastFixTime = 0;
float lastKnownLat = 0;
float lastKnownLon = 0;
bool smsReady = false;

// Store RFID UID and tap count
String lastCardUID = "";
int tapCount = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Setup modem
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

St = rtc.getTime();

String timestamp = String(t.year) + "-" +
                   String(t.mon) + "-" +
                   String(t.date) + " " +
                   String(t.hour) + ":" +
                   String(t.min) + ":" +
                   String(t.sec);

enum TransactionStage {
  PENDING_STATUS,
  IN_PROGRESS,
  COMPLETE // Add this in your enum or declare it
};
enum PassengerType {
  REGULAR,
  STUDENT,
  SENIOR,
  PWD
};
struct Passenger {
  String uid;
  PassengerType type;          // 👈 Added
  float balance;               // 👈 Added
  String tapInLocation;
  unsigned long tapInTime;
  float tapInLat;
  float tapInLon;
  int tapCount;  // New member to count taps
  unsigned long tapOutTime;
  float tapOutLat;
  float tapOutLon;
};
struct PassengerTransaction {
  String uid;
  String tapInLocation;
  String tapOutLocation;
  unsigned long tapInTime;
  unsigned long tapOutTime;
  TransactionStage stage;
};
struct Transaction {
  String uid;
  unsigned long tapInTime;
  int stage;
  unsigned long tapOutTime;
};
struct SMSMessage {
  String number;
  String message;
  int retryCount = 0;
};
struct PassengerProfile {
  String uid;
  String type;      // "student", "senior", "pwd", or "regular"
  int balance;      // Stored in pesos
};
std::vector<PassengerProfile> passengerProfiles;
std::vector<SMSMessage> smsQueue;
bool isSendingSMS = false;
// Global vector for storing dynamic transactions
std::vector<Passenger> onboardPassengers;
std::vector<Transaction> transactions;  // Declare this globally or in the appropriate class.

String currentUID = "";  // Used to track the currently active RFID UID

int findPassengerIndex(const String& uid) {
  for (int i = 0; i < onboardPassengers.size(); i++) {
      if (onboardPassengers[i].uid == uid) {
          return i;
      }
  }
  return -1;
}
int findTransaction(const String& uid) {
  for (int i = 0; i < transactions.size(); i++) {
    if (transactions[i].uid == uid) return i;
  }
  return -1;
} 
PassengerType getPassengerType(String uid) {
  if (uid == "2AD28600") return STUDENT;
  if (uid == "42977755") return SENIOR;
  if (uid == "0576A489") return STUDENT;
  if (uid == "530d6050") return SENIOR;
  if (uid == "4C174402") return REGULAR;
  if (uid == "B67F4B03") return REGULAR;
  return REGULAR;
}
int getInitialBalance(String uid) {
  if (uid == "2AD28600") return 500;
  if (uid == "42977755") return 500;
  if (uid == "0576A489") return 300;
  if (uid == "530d6050") return 300;
  if (uid == "4C174402") return 1000;
  if (uid == "B67F4B03") return 300;
}
Passenger createPassenger(String uid) {
  Passenger p;
  p.uid = uid;
  p.type = getPassengerType(uid);
  p.balance = getInitialBalance(uid);
  return p;
}
bool writeBalance(String uid, int newBalance) {
  for (auto& profile : passengerProfiles) {
    if (profile.uid == uid) {
      profile.balance = newBalance;
      return true;
    }
  }
  return false;
}
void setupPassengerProfiles() {
  passengerProfiles.push_back({"2AD28600", "Student", 500});
  passengerProfiles.push_back({"42977755", "Senior", 500});
  passengerProfiles.push_back({"0576A489", "Student", 300});
  passengerProfiles.push_back({"530d6050", "Senior", 300});
  passengerProfiles.push_back({"B67F4B03", "Regular", 300});
  passengerProfiles.push_back({"4C174402", "Regular", 1000});
}

String getCardTypeFromRFID(String uid) {
  PassengerType type = getPassengerType(uid);
  switch (type) {
    case STUDENT: return "student";
    case SENIOR: return "senior";
    case PWD: return "pwd";
    case REGULAR: return "regular";
  }
}
String getPhoneNumberFromCard(String uid) {
  if (uid.endsWith("8600")) return "09075993391";
  if (uid.endsWith("7755")) return "09856164138";
  if (uid.endsWith("A489")) return "09100300951";
  if (uid.endsWith("6050")) return "09943795023";
  if (uid.endsWith("4B03")) return "09856164138";
  if (uid.endsWith("4402")) return "09100300951";
  return "09100300951"; // Default fallback number
}
String getLocationString(float lat, float lon) {
  return "Lat: " + String(lat, 6) + ", Lon: " + String(lon, 6);
}
int getTapCount(String uid) {
  for (int i = 0; i < onboardPassengers.size(); i++) {
    if (onboardPassengers[i].uid == uid) {
      return onboardPassengers[i].tapCount;
    }
  }
  return 0;  // Return 0 if UID is not found in onboard passengers list
}
void buzzOnce(int duration = 100) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}
void buzzTwice(int duration = 100, int gap = 100) {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    if (i == 0) delay(gap); // Delay only after first buzz
  }
}
void showSuccess() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
  buzzOnce();
  delay(300);  // keep it short
  digitalWrite(LED_GREEN, LOW);
}
void showStandby(){
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}
void showError() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);
  buzzTwice();
  delay(300);
  digitalWrite(LED_RED, LOW);
}
void lcdPrintCentered(String message, int row = 0) {
  int spaces = (16 - message.length()) / 2;
  if (spaces < 0) spaces = 0;
  lcd.setCursor(spaces, row);
  lcd.print(message);
}
bool sendSMS(String message, String number) {
  if (modem.sendSMS(number.c_str(), message.c_str())) {
    return true;
  }
  Serial.println("❌ SMS sending failed (sendSMS returned false)");
  return false;
}

// Function to send SMS asynchronously
void sendSMSAsync() {
  if (!smsReady) {
    Serial.println("❌ SMS not ready yet.");
    return;
  }

  if (!smsQueue.empty()) {
    SMSMessage &sms = smsQueue.front();  // Get first message in queue
    Serial.println("📤 Sending SMS to: " + sms.number);
    Serial.println("📨 Message:\n" + sms.message);
  
    if (sendSMS(sms.number, sms.message)) {  // your GSM SMS send function
      Serial.println("✅ SMS sent successfully.");
      smsQueue.erase(smsQueue.begin());  // Remove sent message
    } else {
      Serial.println("❌ Failed to send SMS.");
      sms.retryCount++;
  
      if (sms.retryCount >= 3) {  // Limit retry attempts
        Serial.println("⚠️ Max retry reached. Dropping SMS.");
        smsQueue.erase(smsQueue.begin());  // Drop the message from queue
      }
      delay(2000);  // Optional: add delay between retries
    }
  }  
}
String getLocationString() {
  float lat = 0, lon = 0;
  int yr = 0, mon = 0, day = 0, hr = 0, min = 0, sec = 0;
  uint8_t fixMode = 0;

  Serial.println("📡 Getting GPS...");
  
  // Try for up to 2 minutes (120 seconds)
  for (int i = 0; i < 40; i++) {
    if (modem.getGPS(&fixMode, &lat, &lon, NULL, NULL, NULL, NULL, NULL, &yr, &mon, &day, &hr, &min, &sec)) {
      if (fixMode == 1 || fixMode == 2) { // 1=2D fix, 2=3D fix
        String location = "📍 Location:\nLat: " + String(lat, 6) + "\nLon: " + String(lon, 6);
        location += "\nDate: " + String(yr) + "-" + String(mon) + "-" + String(day);
        location += "\nTime: " + String(hr) + ":" + String(min) + ":" + String(sec);
        return location;
      }
    }
    delay(3000); // Wait 3 seconds between tries
    Serial.print(".");
  }
  return "❌ GPS unavailable or no fix acquired.";  // Return a meaningful message
}
bool isWaiting = false;
String lastUID = "";
int passengerCount = 0;  // To track the number of onboard passengers

bool getGPSCoordinates(float &lat, float &lon) {
  modem.enableGPS();
  uint8_t fixMode;
  int yr, mon, day, hr, min, sec;
  
  for (int i = 0; i < 30; i++) {  // Try for 60 seconds (30 * 2s)
    if (modem.getGPS(&fixMode, &lat, &lon, nullptr, nullptr, nullptr, nullptr, nullptr, &yr, &mon, &day, &hr, &min, &sec)) {
      if (fixMode > 0) {
        return true;  // GPS fix successful
      }
    }
    delay(2000);  // Wait 2 seconds before next attempt
  }

  return false;  // No GPS fix
}
void updateGPSCache() {
  if (millis() - gpsRefreshTime < 10000) return;  // update every 5s

  float lat, lon;
  uint8_t fixMode;
  if (modem.getGPS(&fixMode, &lat, &lon)) {
    if (fixMode > 0) {
      lastKnownLat = lat;
      lastKnownLon = lon;
      lastFixTime = millis();
      Serial.println("📡 GPS cache updated.");

      if (!smsReady) {
        Serial.println("📲 Initializing SMS mode...");
        modem.sendAT("+CMGF=1");     // Set to text mode
        modem.sendAT("+CNMI=2,1,0,0,0"); // Basic notification setup
        smsReady = true;
      }
    }
  }

  gpsRefreshTime = millis();
}
void setup() {
  Serial.begin(115200);
  modem.begin();
  SPI.begin();
  rfid.PCD_Init();
  rtc.begin();


#ifdef BOARD_POWERON_PIN
  pinMode(BOARD_POWERON_PIN, OUTPUT);
  digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

  pinMode(MODEM_RESET_PIN, OUTPUT);
  digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL); delay(100);
  digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL); delay(2600);
  digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

  pinMode(BOARD_PWRKEY_PIN, OUTPUT);
  digitalWrite(BOARD_PWRKEY_PIN, LOW); delay(100);
  digitalWrite(BOARD_PWRKEY_PIN, HIGH); delay(1000);
  digitalWrite(BOARD_PWRKEY_PIN, LOW);

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

  Serial.println("🔄 Initializing modem...");
  int retry = 0;
  while (!modem.testAT(1000)) {
    if (++retry > 10) {
      digitalWrite(BOARD_PWRKEY_PIN, LOW); delay(100);
      digitalWrite(BOARD_PWRKEY_PIN, HIGH); delay(1000);
      digitalWrite(BOARD_PWRKEY_PIN, LOW);
      retry = 0;
    }
  }
  modem.sendAT("+CGNSPWR=1");
  modem.waitResponse(1000);

  // Enable GPS
  Serial.println("\n🛰️ Enabling GPS...");
  while (!modem.enableGPS(MODEM_GPS_ENABLE_GPIO)) {
  }
  Serial.println("\n✅ GPS Enabled");
  Serial.println("⌛ Waiting for GPS fix...");
  bool gpsReady = false;
float lat2 = 0, lon2 = 0, speed2 = 0, alt2 = 0, accuracy2 = 0;
int vsat2 = 0, usat2 = 0, year2 = 0, month2 = 0, day2 = 0;
int hour2 = 0, min2 = 0, sec2 = 0;
uint8_t fixMode = 0;

Serial.println("⌛ Waiting for GPS fix...");
for (int i = 0; i < 15; i++) { // 15 attempts, adjust if needed
  if (modem.getGPS(&fixMode, &lat2, &lon2, &speed2, &alt2,
                   &vsat2, &usat2, &accuracy2,
                   &year2, &month2, &day2,
                   &hour2, &min2, &sec2)) {
    Serial.println("✅ GPS fix acquired");
    Serial.print("FixMode: "); Serial.println(fixMode);
    Serial.print("Latitude: "); Serial.println(lat2, 6);
    Serial.print("Longitude: "); Serial.println(lon2, 6);
    Serial.print("Accuracy: "); Serial.println(accuracy2);
    
    gpsReady = true;
    break;
  } else {
    Serial.println("❌ Failed to get full GPS data, retrying...");
    delay(10000L);
  }
}

if (!gpsReady) {
  Serial.println("\n❌ GPS Fix not acquired. Halting system.");
  while (true);
}
  
  // ✅ Configure SMS Mode
  modem.sendAT("+CMGF=1");  // Set SMS to text mode
  if (modem.waitResponse(1000) == 1) {
    modem.sendAT("+CSCS=\"GSM\"");  // Use GSM character set
    if (modem.waitResponse(1000) == 1) {
      Serial.println("✅ Modem ready for SMS");
    } else {
      Serial.println("❌ Failed to set character set");
    }
  } else {
    Serial.println("❌ Failed to set SMS text mode");
  }

  Wire.begin();
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcdPrintCentered("TAP HERE!", 0);
  
  pinMode (BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  showStandby();
  SPI.begin(18, 19, 23, 5);  // SCK, MISO, MOSI, SS (last one is optional)
  rfid.PCD_Init();
  Serial.println("🚍 RFID Fare System Ready");
}
void handleTapIn(String uid, int count) {
  int index = findPassengerIndex(currentUID);
  if (index != -1 && onboardPassengers[index].tapOutTime == 0) {  
    Serial.println("⚠️ Already tapped in!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Already tapped in!");
    showError();
    delay(1000);  // Wait for a moment before switching to standby mode
    showStandby();// Blue LED for standby
    return;
  }

  float baseFare = 15.0;
  int balance = getInitialBalance(uid);  // Read card balance

if (balance < baseFare) {
    Serial.println("❌ Insufficient balance to tap in.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("❌ Insufficient");
    lcd.setCursor(0, 1);
    lcd.print("Balance: ₱" + String(balance));
    showError();
    return;  // Prevent tap-in
}
  float lat = lastKnownLat;
  float lon = lastKnownLon;

  Passenger p = createPassenger(currentUID);
  p.uid = uid;
  p.tapInTime = millis();
  p.tapInLat = lat;
  p.tapInLon = lon;
  p.tapCount = tapCount;

  onboardPassengers.push_back(p);
  passengerCount++;
  
  Serial.println("✅ Tap-In recorded");
  Serial.print("Type: "); Serial.println(getCardTypeFromRFID(uid));
  Serial.println("Balance: " + String(p.balance));
  Serial.println("Tap-In Location:");
  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lon, 6);
  showSuccess();
  delay(1000); 
  showStandby();

  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Tap Here! ");

  Transaction transaction = {uid, millis(), IN_PROGRESS};
  transactions.push_back(transaction);

  rfid.PICC_HaltA(); 
  rfid.PCD_StopCrypto1();
  }

void removeTransaction(int index) {
  transactions.erase(transactions.begin() + index);
}
void handleTapOut(String uid) {
  int index = findPassengerIndex(uid);
  if (index == -1) {
    Serial.println("❌ Tap-Out failed: Passenger UID not found in onboard list.");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("No Tap-In Found");
    showError();
    return;
  }
  Passenger p = onboardPassengers[index];
  int tapCount = p.tapCount;

  int tapInIndex = getNearestBusStopIndex(p.tapInLat, p.tapInLon);
  int tapOutIndex = getNearestBusStopIndex(lastKnownLat, lastKnownLon);
  String cardType = getCardTypeFromRFID(uid); // Implement this based on your card type (e.g., Regular, Senior)
 
  // Calculate fare based on tap count
  float fare = calculateFare(tapInIndex, tapOutIndex, cardType, tapCount);
  int balance = getInitialBalance(uid);  // Get initial balance

  if (balance < fare) {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Insufficient Balance");
  showError();
  return;
}
  int newBalance = balance - fare * tapCount;

  // Update passenger info on tap-out
  float lat = lastKnownLat;
  float lon = lastKnownLon;

  p.uid = uid;
  p.tapInTime = millis();  // Set the tap-out time (optional, depending on your use case)
  p.tapInLat = lat;        // Store tap-out location coordinates
  p.tapInLon = lon;

  // Remove the passenger from the onboard list
  onboardPassengers.erase(onboardPassengers.begin() + index);
  passengerCount--;

  // LCD Success
  lcd.clear();
  lcd.backlight();
  lcdPrintCentered("TAP OUT", 0);
  lcdPrintCentered("SUCCESSFUL!", 1);
  showSuccess();
  delay (1000);
  showStandby();

  Serial.println("✅ Tap-Out Success");
  Serial.println("📍 Tap-Out Location:");
  Serial.print("📍 Latitude: ");
  Serial.println(String(lat, 5));
  Serial.print("📍 Longitude: ");
  Serial.println(String(lon, 5));
  Serial.println("Remaining Balance:");
  Serial.println(String(newBalance));

// Handle transaction completion
  int transactionIndex = findTransaction(uid);
  if (transactionIndex != -1) {
    transactions[transactionIndex].tapOutTime = millis(); // Set Tap Out Time
    transactions[transactionIndex].stage = COMPLETE; // Mark transaction as complete
  }
  // Send SMS receipt
  String gpsData = "Lat: " + String(lat, 6) + " Lon: " + String(lon, 6);
  // Compose SMS message
String phoneNumber = getPhoneNumberFromCard(uid);
String smsText = 
"SMART FARE E-TICKET\n"
"UID: " + uid.substring(uid.length() - 7) + "\n" +
"Fare:" + String(fare, 2) + "php" +"\n" +
"Balance: " + String(newBalance) + "php" + "\n" +
"From: " + getLocationString(p.tapInLat, p.tapInLon) + "\n" +
"To: " + getLocationString(lat, lon);
"Time: " + timestamp;

// Assuming there’s a function that sends the SMS
SMSMessage sms;
sms.number = getPhoneNumberFromCard(uid);  // Implement this function
sms.message = smsText;
smsQueue.push_back(sms);
Serial.println("SMS Queue size: " + String(smsQueue.size()));
  // Update transaction stage
  if (transactionIndex != -1) {
    removeTransaction(transactionIndex);
  }

  // Halt RFID communication
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

 void checkTapWindow() {
  if (lastUID != "" && (millis() - lastTapTime) > 5000 && tapCount > 0) {
    Serial.println("⏱️ Tap window ended. Final count: " + String(tapCount));

    // ✅ Check if UID already tapped in
    if (findPassengerIndex(lastUID) != -1) {
      Serial.println("🚪 Tap-Out Detected for UID: " + lastUID);
      handleTapOut(lastUID);
    } else {
      Serial.println("🛑 Tap-In Detected for UID: " + lastUID);
      handleTapIn(lastUID, tapCount);  // ✅ correct
    }

    lastUID = "";
    tapCount = 0;
  }
}
unsigned long lastGPSUpdate = 0;

void loop() {
  updateGPSCache();
  currentTime = millis();
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    int index = findPassengerIndex(uid);
    Serial.println("✅ UID: " + uid);
    showSuccess;

    if (lastUID == uid) {
      // Same card tapped again
      lastTapTime = currentTime;
      tapCount++;
      Serial.println("🔁 Same card tapped again. Count: " + String(tapCount));
    } else if (lastUID == "" || (currentTime - lastTapTime) > 5000) {
      // New card or timeout passed
      lastUID = uid;
      tapCount = 1;
      lastTapTime = currentTime;
      Serial.println("✅ New card detected: " + uid);
    } else {
      // Different card during active window, ignore
      Serial.println("🚫 Another card tried during active window.");
      rfid.PICC_HaltA(); 
      rfid.PCD_StopCrypto1();
      return;
    }

    // Update LCD
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("UID: " + uid);
    lcd.setCursor(0, 1);
    lcd.print("Taps: " + String(tapCount));

    rfid.PICC_HaltA(); 
    rfid.PCD_StopCrypto1();
  }
  if (millis() - lastGPSUpdate > 1000) {
    updateGPSCache();
    lastGPSUpdate = millis();
  }
  checkTapWindow(); // Check if the 5-second window has ended
 
  sendSMSAsync();
}
