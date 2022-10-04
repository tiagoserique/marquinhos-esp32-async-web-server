#include <Arduino.h>
#include <MarquinhosWebServer.h>
#include <QTRSensors.h>


// flags for tests =============================================================
#define MONITOR_FLAG true   // set to false to disable serial output
#define MAX_CLIENTS 4       // max number of clients connected to the server
#define HIDE_SSID false     // hide the ssid from the wifi list


// Variables for constraints control ===========================================

// kp is [0], kd is [1], ki is [2];
double k[3] = {
    0,          // kp
    0,          // kd
    0,          // ki
};


// Variables for speed control =================================================

// maxSpeed is [0], minSpeed is [1];
int speed[2] = {
    0,          // maxSpeed
    0,          // minSpeed
};


// Curve sensors ============================================================

QTRSensors qtr;

int curve_sensor01 = 18;
int curve_sensor02 = 21;

// variables for values that will be sent to the app
extern int curve_sensor01_value;
extern int curve_sensor02_value;

extern const uint8_t SensorCount = 8;
extern uint16_t sensorValues[SensorCount];


void setup() {
    Serial.begin(9600);
    delay(100);


    initWebServer(MONITOR_FLAG, MAX_CLIENTS, HIDE_SSID);


    // SENSORS CONFIGURATION ===================================================
    // configure the sensors
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){13, 14, 27, 26, 25, 33, 32, 19}, SensorCount);
    qtr.setEmitterPin(4);

    delay(500);

    // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
    // = ~25 ms per calibrate() call.
    // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 400; i++){
        qtr.calibrate();
    }

    // print the calibration minimum values measured when emitters were on
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
    }
    Serial.println();

    // print the calibration maximum values measured when emitters were on
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
    }
    Serial.println();
    Serial.println();


    delay(1000);
}


void loop(){
    // read calibrated sensor values and obtain a measure of the line position
    // from 0 to 5000 (for a white line, use readLineWhite() instead)
    uint16_t position = qtr.readLineWhite(sensorValues);

    // print the sensor values as numbers from 0 to 1000, where 0 means maximum
    // reflectance and 1000 means minimum reflectance, followed by the line
    // position
    for (uint8_t i = 0; i < SensorCount; i++){
        Serial.print(sensorValues[i]);
        Serial.print('\t');
    }
    Serial.println(position);

    curve_sensor01_value = digitalRead(curve_sensor01);
    curve_sensor02_value = digitalRead(curve_sensor02);

    Serial.println(curve_sensor01_value);
    Serial.println(curve_sensor02_value);


    delay(250);
}
