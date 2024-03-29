#include <MarquinhosWebServer.h>


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

String hostname = "marquinhost";


// Curve sensors ============================================================

// variables for values that will be sent to the app
int curve_sensor01_value;
int curve_sensor02_value;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];


// Initialize the input fields =================================================

const char *PARAM_KP = "kp";
const char *PARAM_KD = "kd";
const char *PARAM_KI = "ki";
const char *PARAM_MAX_SPEED = "max_speed";
const char *PARAM_MIN_SPEED = "min_speed";


// HTML web page to handle 5 input fields ======================================

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

            <button><a href="/data">DATA JSON</a></button>
        </body>
    </html>
)rawliteral";



void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void initWebServer(bool output = false, int max_clients = 1, bool hide_ssid = false) {
    // AP CONFIGURATION ========================================================
    
    if ( output ) {
        Serial.println();
        Serial.println();
        Serial.println("Setting AP (Access Point)...");
        Serial.print("SSID: ");
        Serial.print(ssid);
        Serial.print(" | pswd: ");
        Serial.print(password);
        Serial.print(" | hostname: ");
        Serial.println(hostname.c_str());
        Serial.println();
    }


    // Set the AP configuration and start the AP ===============================
    
    if ( !output ) {
        WiFi.softAPConfig(ip, gateway, subnet);
        WiFi.softAPsetHostname(hostname.c_str());
        WiFi.softAP(ssid, password, 1, hide_ssid, max_clients);
    }
    else {
        Serial.print("Configuring AP... ");
        Serial.println(
            WiFi.softAPConfig(ip, gateway, subnet) ? "Ready" : "Failed!"
        );


        Serial.print("Setting hostname... ");
        Serial.println(
            WiFi.softAPsetHostname(hostname.c_str()) ? "Ready" : "Failed!"
        );


        Serial.print("Starting AP... ");
        Serial.println(
            WiFi.softAP(ssid, password, 1, hide_ssid, max_clients) ? "Ready" : "Failed!"
        );
        
        Serial.println();
    }
    
    
    // Print the IP address ================================================
    
    if ( !output ) {
        WiFi.softAPIP();
    }
    else {
        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
        Serial.println();
    }


    // Define routes =======================================================

    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });

    // Send a GET request to <ESP_IP>/get?<inputParam>=<inputMessage>
    server.on("/get", HTTP_GET, [output](AsyncWebServerRequest *request) {
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
        

        bool hasKp = request->hasParam(PARAM_KP); 
        bool hasKd = request->hasParam(PARAM_KD); 
        bool hasKi = request->hasParam(PARAM_KI); 
        bool hasMaxSpeed = request->hasParam(PARAM_MAX_SPEED); 
        bool hasMinSpeed = request->hasParam(PARAM_MIN_SPEED); 


        if ( !hasKp && !hasKd && !hasKi && !hasMaxSpeed && !hasMinSpeed ){
            String inputMessage = "No message sent";
            String inputParam = "none";

            if ( output ) {
                Serial.println(inputMessage);
                Serial.println();
            }

            request->send(
                200, 
                "text/html", 
                "HTTP GET request sent to your ESP on input field (" 
                                + inputParam + ") with value: " + inputMessage +
                                "<br><a href=\"/\">Return to Home Page</a>"
            );
        }
        else {
            String response = "HTTP GET request sent to your ESP on input field <br>";

            if ( output ){
                for (int i = 0; i < inputMessage.length(); i++){
                    Serial.print("INPUT: ");
                    Serial.print(inputParam.nth(i)->c_str());
                    Serial.print(" = ");
                    Serial.println(inputMessage.nth(i)->c_str());
                }
                Serial.println();
            }

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

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {

        DynamicJsonDocument doc(1024);
        doc["curve_01"] = curve_sensor01_value;
        doc["curve_02"] = curve_sensor01_value;
        doc["max_speed"] = speed[0];
        doc["min_speed"] = speed[1];
        doc["sensor_0"] = sensorValues[0];
        doc["sensor_1"] = sensorValues[1];
        doc["sensor_2"] = sensorValues[2];
        doc["sensor_3"] = sensorValues[3];
        doc["sensor_4"] = sensorValues[4];
        doc["sensor_5"] = sensorValues[5];
        doc["sensor_6"] = sensorValues[6];
        doc["sensor_7"] = sensorValues[7];

        String jsonOutput;
        serializeJson(doc, jsonOutput);

        request->send(200, "application/json", jsonOutput);
    });


    // Start the server ====================================================
    
    server.onNotFound(notFound);
    server.begin();

    if ( !output ) {
        MDNS.begin(hostname.c_str());
    }
    else {
        Serial.print("Starting DNS server... ");
        Serial.print(
            MDNS.begin(hostname.c_str()) ? "Ready" : "Failed!"
        );
        Serial.println();
        Serial.println("=====================================");
        Serial.println();
    }
}

