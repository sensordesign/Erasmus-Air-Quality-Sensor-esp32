/*
                   # THIS SKETCH IS NOT NEEDED WITH AUTOCALIBRATION #

    Unlike typical sensors, calibration() refers to the zero point where CO2 is 400ppm.
    This 400ppm comes from the average atmospheric value of 400ppm (or atleast was).

    Depending on the sensor model, the hardcoded value can usually be found by
    calling getBackgroundCO2();

    So if you intend to manually calibrate your sensor, it's usually best to do so at
    night and outside after 20 minutes of run time.

    Instead if you're using auto calibration, then the sensor takes the lowest value observed
    in the last 24 hours and adjusts it's self accordingly over a few weeks.

    HOW TO USE:

    ----- Hardware Method  -----
    By pulling the zero HD low (0V) for 7 seconds as per the datasheet.

    ----- Software Method -----
    Run this sketch, disconnect MHZ19 from device after sketch ends (20+ minutes) and upload new
    code to avoid recalibration.

    ----- Auto calibration ----
    As mentioned above if this is set to true, the sensor will adjust it's self over a few weeks
    according to the lowest observed CO2 values each day. *You don't need to run this sketch!

*/

#include <Arduino.h>
#include "MHZ19.h"

#define RX_PIN 16
#define TX_PIN 17
#define BAUDRATE 9600

MHZ19 myMHZ19;
#if defined(ESP32)
HardwareSerial mySerial(2);                                // On ESP32 we do not require the SoftwareSerial library, since we have 2 USARTS available
#else
#include <SoftwareSerial.h>                                //  Remove if using HardwareSerial or non-uno compatible device
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial
#endif

unsigned long timeElapse = 0;

void setup()
{
    Serial.begin(9600);

    mySerial.begin(BAUDRATE,SERIAL_8N1,RX_PIN,TX_PIN);    // sensor serial
    myMHZ19.begin(mySerial);     // pass to library

    myMHZ19.autoCalibration(false);     // make sure auto calibration is off
    Serial.print("ABC Status: "); myMHZ19.getABC() ? Serial.println("ON") :  Serial.println("OFF");  // now print it's status

    Serial.println("Waiting 20 minutes to stabilize...");
   /* if you don't need to wait (it's already been this amount of time), remove the 2 lines */
    timeElapse = 12e5;   //  20 minutes in milliseconds
    delay(timeElapse);    //  wait this duration

    Serial.println("Calibrating..");
    myMHZ19.calibrate();    // Take a reading which be used as the zero point for 400 ppm

}

void loop()
{
    if (millis() - timeElapse >= 2000)  // Check if interval has elapsed (non-blocking delay() equivalent)
    {
        int CO2;
        CO2 = myMHZ19.getCO2();

        Serial.print("CO2 (ppm): ");
        Serial.println(CO2);

        int8_t Temp;    // Buffer for temperature
        Temp = myMHZ19.getTemperature();    // Request Temperature (as Celsius)

        Serial.print("Temperature (C): ");
        Serial.println(Temp);

        timeElapse = millis();  // Update interval
    }
}
