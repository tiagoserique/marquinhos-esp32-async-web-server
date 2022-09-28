#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <Arduino.h>
#include <PololuQTRSensors.h>

// Defines the number of sensors and the pins used for the QTR sensors
#define NUM_SENSORS 8
#define EMITTER_PIN 5

// Defines the time for sensor output to go low
#define TIMEOUT 2500

// Determines the number of iterations at calibration mode
#define CALIBRATION_ITERATIONS 400

// Set the read line mode
#define WHITE_LINE true

// Instantiates the QTR sensors
// A0, A1, A2, A3, A4, A5, A6, 
PololuQTRSensorsAnalog sensor_array((unsigned char[]) {
        2,
        4, 
        25, 
        26, 
        27, 
        14, 
        12,
        13,
    }, NUM_SENSORS, 4, QTR_NO_EMITTER_PIN);

unsigned int sensor_values[NUM_SENSORS];

#endif