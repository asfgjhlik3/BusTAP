#ifndef GSM_UTILS_H
#define GSM_UTILS_H

#include <HardwareSerial.h>

// Assumes SerialMon and SerialAT are defined in main.cpp

inline void sendATCommand(const char* cmd, HardwareSerial& modemSerial = SerialAT, HardwareSerial& debugSerial = Serial) {
  debugSerial.print("Sending command: ");
  debugSerial.println(cmd);
  modemSerial.println(cmd);
  delay(300);
  while (modemSerial.available()) {
    debugSerial.write(modemSerial.read());
  }
  debugSerial.println();
}

#endif
