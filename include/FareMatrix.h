#ifndef FARE_MATRIX_H
#define FARE_MATRIX_H

#include <Arduino.h>

struct BusStop {
    String label;
    String name;
    float latitude;
    float longitude;
};

int getNearestBusStopIndex(float lat, float lon);
int getFareBetweenStops(int fromIndex, int toIndex);
extern const float fareMatrix_Bayanihan_to_IBJT[21][21];
extern const float fareMatrix_IBJT_to_Bayanihan[21][21];
float calculateFare(int tapInIndex, int tapOutIndex, String cardType, int tapCount);

extern const BusStop busStops[];
extern const int NUM_BUS_STOPS;

#endif // FARE_MATRIX_H
