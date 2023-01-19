#ifndef Sensor_retrievement
#define Sensor_retrievement
#include <Arduino.h>
#include "DHT.h"
#include <SoftwareSerial.h>

void instruments_setup();
double temperature();
double co2();
double humidity();
int pm10();
int pm2_5();
int readco2();
int sds_loop();
#endif
