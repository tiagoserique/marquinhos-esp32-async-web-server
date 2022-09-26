#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


// #define MONITOR_FLAG

// Wifi credentials ============================================================
const char *ssid = "marquinhos-yapira";
const char *password = "zeruela01";


// Configuration for the AP ====================================================
IPAddress ip(192,168,168,168);
IPAddress gateway(192,168,168,254);
IPAddress subnet(255,255,255,0);


// Create server on port 80 ====================================================
AsyncWebServer server(80);


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


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void setup() {
    #ifdef MONITOR_FLAG
        Serial.begin(115200);
        delay(10);
        Serial.println();
        Serial.println();
        Serial.println("Setting AP (Access Point)...");
        Serial.print("SSID: ");
        Serial.print(ssid);
        Serial.print(" | pswd: ");
        Serial.println(password);
        Serial.println();
    #endif

    // Set the AP configuration and start the AP ===============================
    #ifdef MONITOR_FLAG
        Serial.print("Configuring AP... ");
        Serial.println(
            WiFi.softAPConfig(ip, gateway, subnet) ? "Ready" : "Failed!"
        );
        Serial.print("Starting AP... ");
        Serial.println(
            WiFi.softAP(ssid, password, 1, 0, 1) ? "Ready" : "Failed!"
        );
        Serial.println();
    #else
        WiFi.softAPConfig(ip, gateway, subnet);
        WiFi.softAP(ssid, password, 1, 0, 1);
    #endif
    
    
    // Print the IP address ====================================================
    #ifdef MONITOR_FLAG
        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
        Serial.println();
        Serial.println("=====================================");
        Serial.println();
    #else
        WiFi.softAPIP();
    #endif
    

    // Start the server ========================================================

    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    // Send a GET request to <ESP_IP>/get?<inputParam>=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        StringArray inputMessage;
        StringArray inputParam;

        if ( request->hasParam(PARAM_KP) ) {
            String value = request->getParam(PARAM_KP)->value();
            inputMessage.add(value);
            inputParam.add(PARAM_KP);
            k[0] = value.toDouble();
        }
        
        if ( request->hasParam(PARAM_KD) ) {
            String value = request->getParam(PARAM_KD)->value();
            inputMessage.add(value);
            inputParam.add(PARAM_KD);
            k[1] = value.toDouble();
        }
        
        if ( request->hasParam(PARAM_KI) ) {
            String value = request->getParam(PARAM_KI)->value();
            inputMessage.add(value);
            inputParam.add(PARAM_KI);
            k[2] = value.toDouble();
        }
        
        if ( request->hasParam(PARAM_MAX_SPEED) ) {
            String value = request->getParam(PARAM_MAX_SPEED)->value();
            inputMessage.add(value);
            inputParam.add(PARAM_MAX_SPEED);
            speed[0] = value.toInt();
        }
        
        if ( request->hasParam(PARAM_MIN_SPEED) ) {
            String value = request->getParam(PARAM_MAX_SPEED)->value();
            inputMessage.add(value);
            inputParam.add(PARAM_MIN_SPEED);
            speed[1] = value.toInt();
        }
        
        if ( !request->hasParam(PARAM_KP) &&  !request->hasParam(PARAM_KD) && 
            !request->hasParam(PARAM_KI) && !request->hasParam(PARAM_MAX_SPEED) && 
            !request->hasParam(PARAM_MIN_SPEED)
        ){
            String inputMessage = "No message sent";
            String inputParam = "none";

            #ifdef MONITOR_FLAG
                Serial.println(inputMessage);
            #endif

            request->send(
                200, 
                "text/html", 
                "HTTP GET request sent to your ESP on input field (" 
                                + inputParam + ") with value: " + inputMessage +
                                "<br><a href=\"/\">Return to Home Page</a>"
            );
        }
        else {
            #ifdef MONITOR_FLAG
                for (int i = 0; i < inputMessage.length(); i++){
                    Serial.print("INPUT: ");
                    Serial.print(inputParam.nth(i)->c_str());
                    Serial.print(" = ");
                    Serial.println(inputMessage.nth(i)->c_str());
                }
                Serial.println();
            #endif

            String response = "HTTP GET request sent to your ESP on input field <br>";
            for (int i = 0; i < inputMessage.length(); i++){
                response += "<strong>";
                response += inputParam.nth(i)->c_str();
                response += "</strong>";
                response += " with value: ";
                response += inputMessage.nth(i)->c_str();
                response += "<br>";
            }
            response += "<br><a href=\"/\">Return to Home Page</a>";

            request->send(200, "text/html", response);
        }
    
        
    });
    server.onNotFound(notFound);

    server.begin();
}


void loop(){
    #ifdef MONITOR_FLAG
        for (int i = 0; i < 3; i++){
            Serial.print(k[i]);
            Serial.print(" ");
        }

        for (int i = 0; i < 2; i++){
            Serial.print(speed[i]);
            Serial.print(" ");
        }
    #endif
}
