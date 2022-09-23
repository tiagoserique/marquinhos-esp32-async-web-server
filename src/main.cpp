#include <Arduino.h>
#include <WiFi.h>


// Wifi credentials
const char* ssid = "marquinhos-yapira";
const char* password = "zeruela";


// Configuration for the AP
IPAddress ip(192,168,168,168);
IPAddress gateway(192,168,168,254);
IPAddress subnet(255,255,255,0);


// Create server on port 80
WiFiServer server(80);


void setup() {
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


    // Set the AP configuration and start the AP
    Serial.println(
        WiFi.softAPConfig(ip, gateway, subnet) ? "Ready" : "Failed!"
    );
    Serial.println(
        WiFi.softAP(ssid, password, 1, 0, 1) ? "Ready" : "Failed!"
    );
    
    
    // Print the IP address
    Serial.println(WiFi.softAPIP());

    // Start the server
    server.begin();
}


void loop() {}
