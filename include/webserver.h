#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>


// Wifi credentials ============================================================
const char *ssid = "marquinhos-yapira";
const char *password = "zeruela01";


// Configuration for the AP ====================================================
IPAddress ip(192,168,168,168);
IPAddress gateway(192,168,168,254);
IPAddress subnet(255,255,255,0);


// Create server on port 80 ====================================================
AsyncWebServer server(80);

// Create DNS server ===========================================================
DNSServer dnsServer;
String hostname = "marquinhost";


// Variables for constraints control ===========================================
// kp is [0], kd is [1], ki is [2];
double k[3] = {0, 0, 0};

// maxSpeed is [0], minSpeed is [1];
int speed[2];


// Initialize the input fields =================================================
const char *PARAM_KP = "kp";
const char *PARAM_KD = "kd";
const char *PARAM_KI = "ki";
const char *PARAM_MAX_SPEED = "max_speed";
const char *PARAM_MIN_SPEED = "min_speed";


// HTML web page to handle 3 input fields (input1, input2, input3) =============
const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html>
        <head>
            <title>ESP Input Form</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
        </head>
        <body>
            <form action="/get">
                kp: <input type="text" name="kp">
                
                <br>
                
                kd: <input type="text" name="kd">

                <br>

                ki: <input type="text" name="ki">

                <br>

                Max Speed: <input type="text" name="max_speed">

                <br>

                Min Speed: <input type="text" name="min_speed">

                <br>

                <input type="submit" value="Submit">
            </form>
        </body>
    </html>
)rawliteral";


#endif // __WEB_SERVER_H__