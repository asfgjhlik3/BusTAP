#include "FareMatrix.h"
#define BUS_STOP_COUNT 21
// 21 bus stops for example
const BusStop busStops[21] = {
    {"Bayanihan", "Bayanihan", 8.19636, 124.22690},
    {"Tabay 1,2", "TABAY 1,2", 8.20485, 124.22463,},
    {"IBJT South-NSC", "IBJT SOUTH-NSC", 8.20744, 124.21641},
    {"Shell Tominobo", "SHELL TOMINOBO", 8.20969, 124.21879},
    {"IMDI Overpass", "IMDI OVERPASSS", 8.21063, 124.22201},
    {"Petron Tubod", "PETRON TUBOD", 8.21654, 124.23866},
    {"Mercy Junction", "MERCY JUNCTION", 8.21537, 124.23146},
    {"Petron Camague", "PETRON CAMAGUE", 8.21254, 124.22707},
    {"Robinsons Supermarket", "ROBINSONS", 8.21838, 124.24034},
    {"Capitol College", "CAPITOL", 8.22249, 124.24059},
    {"ICNHS", "ICNHS", 8.22579, 124.23994},
    {"Jollibee Aguinaldo", "JOLLIBEE AGUINALDO", 8.22792, 124.24047},
    {"Metrobank", "METROBANK", 8.22984, 124.24083},
    {"Gaisano Mall", "GAISANO MALL", 8.23095, 124.24136},
    {"Kingsway Inn", "KINGSWAY INN", 8.23303, 124.24169},
    {"Solana/SBG Tibanga", "SOLANA /SBGTIBANGA", 8.23692, 124.24383},
    {"MSU-IIT", "MSU-IIT", 8.2109886, 124.2242461},
    {"SSS Iligan Office", "SSS ILIGAN OFFICE", 8.241817, 124.248054},
    {"Franciscan", "Franciscan", 8.243800, 124.250455},
    {"Tambo Overpass", "TAMBO OVERPASS", 8.244840, 124.255514},
    {"IBJT North Tambo", "IBJT NORTH TAMBO", 8.24583, 124.25528},

};
// 21x21 fare matrix (in pesos)
const float fareMatrix_Bayanihan_to_IBJT[21][21] = {
    // Row 0: Bayanihan → [all destinations]
    {15.00, 14.00, 15.00, 15.00, 15.00, 15.00, 15.54, 17.08, 17.74, 20.06, 20.94, 21.38, 21.82, 22.26, 22.70, 23.80, 24.68, 25.78, 26.44, 27.54, 29.96},
    // Row 1: Tabay 1,2 → [all destinations]
    {14.00, 14.00, 14.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.32, 17.20, 17.64, 18.08, 18.52, 18.96, 20.06, 20.94, 22.04, 22.70, 23.80, 26.22},
    // Row 2: IBJT SOUTH-NSC → [all destinations]
    {15.00, 14.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 14.00, 15.22, 15.66, 15.88, 16.54, 17.64, 18.52, 19.62, 20.28, 21.38, 23.80},
    // Row 3: SHELL TOMINOBO → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.44, 15.88, 16.98, 17.86, 18.96, 19.62, 20.72, 23.14},
    // Row 4: IMDI OVERPASS → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.10, 16.98, 18.08, 18.74, 19.84, 22.26},
    // Row 5: PETRON CAMAGUE → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.66, 16.76, 17.42, 18.52, 20.94},
    // Row 6: MERCY JUNCTION → [all destinations]
    {15.54, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.44, 16.10, 17.20, 19.62},
    // Row 7: PETRON TUBOD → [all destinations]
    {17.08, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.66, 18.08},
    // Row 8: ROBINSONS → [all destinations]
    {17.74, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 17.42},
    // Row 9: CAPITOL → [all destinations]
    {20.06, 16.32, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 10: ICNHS → [all destinations]
    {20.94, 17.20, 14.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 11: JOLLIBEE AGUINALDO → [all destinations]
    {21.38, 17.64, 15.22, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 12: METROBANK → [all destinations]
    {21.82, 18.08, 15.66, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 13: GAISANO MALL → [all destinations]
    {22.26, 18.52, 15.88, 15.44, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 14: KINGSWAY INN → [all destinations]
    {22.70, 18.96, 16.54, 15.88, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 15: SOLANA /SBGTIBANGA → [all destinations]
    {23.80, 20.06, 17.64, 16.98, 16.10, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 16: MSU-IIT → [all destinations]
    {24.68, 20.94, 18.52, 17.86, 16.98, 15.66, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 17: SSS ILIGAN OFFICE → [all destinations]
    {25.78, 22.04, 19.62, 18.96, 18.08, 16.76, 15.44, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 18: Franciscan → [all destinations]
    {26.44, 22.70, 20.28, 19.62, 18.74, 17.42, 16.10, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 19: TAMBO OVERPASS → [all destinations]
    {27.54, 23.80, 21.38, 20.72, 19.84, 18.52, 17.20, 15.66, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 20: IBJT NORTH TAMBO → [all destinations]
    {29.96, 26.22, 23.80, 23.14, 22.26, 20.94, 19.62, 18.08, 17.42, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00}
  };
  
  const float fareMatrix_IBJT_to_Bayanihan[21][21] = {
    // Row 0: IBJT NORTH TAMBO → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 18.00, 18.00, 20.00, 21.00, 22.00, 23.00, 24.00, 26.00, 30.00},
    // Row 1: TAMBO OVERPASS → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 17.00, 19.00, 21.00, 21.00, 23.00, 24.00, 28.00},
    // Row 2: Franciscan → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 17.00, 19.00, 20.00, 20.00, 23.00, 26.00},
    // Row 3: SSS ILIGAN OFFICE → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 18.00, 19.00, 19.00, 21.00, 25.00},
    // Row 4: MSU-IIT → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 17.00, 18.00, 18.00, 20.00, 24.00},
    // Row 5: SOLANA / SBG TIBANGA → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 17.00, 17.00, 19.00, 23.00},
    // Row 6: KINGSWAY INN → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 16.00, 19.00, 23.00},
    // Row 7: GAISANO MALL → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 18.00, 22.00},
    // Row 8: METROBANK → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 17.00, 21.00},
    // Row 9: JOLLIBEE AGUINALDO → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00, 20.00},
    // Row 10: ICNHS → [all destinations]
    {15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 19.00},
    // Row 11: CAPITOL → [all destinations]
    {16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 18.00},
    // Row 12: ROBINSONS → [all destinations]
    {18.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 16.00},
    // Row 13: PETRON TUBOD → [all destinations]
    {18.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 14: MERCY JUNCTION → [all destinations]
    {20.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 15: PETRON CAMAGUE → [all destinations]
    {21.00, 19.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 16: IMDI OVERPASS → [all destinations]
    {22.00, 21.00, 19.00, 18.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 17: SHELL TOMINOBO → [all destinations]
    {23.00, 21.00, 20.00, 19.00, 18.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 18: IBJT SOUTH–NSC → [all destinations]
    {24.00, 23.00, 20.00, 19.00, 18.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 19: Tabay 1,2 → [all destinations]
    {26.00, 24.00, 23.00, 21.00, 20.00, 19.00, 19.00, 18.00, 17.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00},
    // Row 20: Bayanihan → [all destinations]
    {30.00, 28.00, 26.00, 25.00, 24.00, 23.00, 23.00, 22.00, 21.00, 20.00, 19.00, 18.00, 16.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00}
  };

float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    float R = 6371.0;  // Earth's radius in km
    float dLat = radians(lat2 - lat1);
    float dLon = radians(lon2 - lon1);
    float a = sin(dLat / 2) * sin(dLat / 2) +
              cos(radians(lat1)) * cos(radians(lat2)) *
              sin(dLon / 2) * sin(dLon / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}
float getFare(int fromStop, int toStop) {
  return fareMatrix_Bayanihan_to_IBJT[fromStop][toStop];
}

float calculateFare(int tapInIndex, int tapOutIndex, String cardType, int tapCount) {
  const int maxStops = 21;  // Adjust according to your matrix size

  // Safety check for valid indices
  if (tapInIndex < 0 || tapOutIndex < 0 || tapInIndex >= maxStops || tapOutIndex >= maxStops) {
    return 0.0;
  }

  float baseFare = 15.0;  // Default minimum fare

  if (tapInIndex != tapOutIndex) {
    if (tapInIndex < tapOutIndex) {
      baseFare = fareMatrix_Bayanihan_to_IBJT[tapInIndex][tapOutIndex];
    } else {
      baseFare = fareMatrix_IBJT_to_Bayanihan[tapInIndex][tapOutIndex];
    }
  }

  cardType.toLowerCase();  // ✅ Modifies cardType in place
  // Apply 20% discount
  if (cardType == "student" || cardType == "pwd" || cardType == "senior") {
    baseFare *= 0.80;
  }

  float totalFare = baseFare * tapCount;
  return totalFare;
}
int getNearestBusStopIndex(float lat, float lon) {
    float minDistance = 1000000;
    int nearestIndex = 0;
  
    for (int i = 0; i < BUS_STOP_COUNT; i++) {
      float d = sqrt(pow(lat - busStops[i].latitude, 2) + pow(lon - busStops[i].longitude, 2));
      if (d < minDistance) {
        minDistance = d;
        nearestIndex = i;
      }
    }
    return nearestIndex;
  }
  int getFareBetweenStops(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < BUS_STOP_COUNT &&
        toIndex >= 0 && toIndex < BUS_STOP_COUNT) {
      return fareMatrix_Bayanihan_to_IBJT[fromIndex][toIndex];
    }
    return 0; // Default fare if invalid
  }  
