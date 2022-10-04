#include <Arduino.h>
#include <MarquinhosWebServer.h>
#include <sensor.h>


// flags for tests =============================================================
// #define SENSORS             // comment this line to disable sensors
#define MONITOR_FLAG true   // set to false to disable serial output
#define MAX_CLIENTS 4       // max number of clients connected to the server
#define HIDE_SSID false     // hide the ssid from the wifi list



void setup() {
    Serial.begin(9600);
    delay(100);


    initWebServer(MONITOR_FLAG, MAX_CLIENTS, HIDE_SSID);


    // SENSORS CONFIGURATION ===================================================
    #ifdef SENSORS
        #ifdef MONITOR_FLAG
            Serial.println();
            Serial.println();
            Serial.println("Setting sensors...");
            Serial.println();
        #endif
        

        // pinMode(17, OUTPUT);
        // digitalWrite(17, HIGH); // Turn on LED to indicate calibration mode
        for(int i = 0; i < CALIBRATION_ITERATIONS; i++){
            sensor_array.calibrate(QTR_EMITTERS_ON);   
        }
        // digitalWrite(17, LOW); // Turn off LED to indicate end of calibration mode 


        for (int i = 0; i < NUM_SENSORS; i++){
            Serial.print(sensor_array.calibratedMinimumOn[i] + "\t");
            Serial.print('\n');
        }
        Serial.println();

        for (int i = 0; i < NUM_SENSORS; i++){
            Serial.print(sensor_array.calibratedMaximumOn[i] + "\t");
            Serial.print('\n');
        }
        Serial.println();
        Serial.println();
    #endif

    delay(1000);
}


void loop(){
}
