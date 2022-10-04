#ifndef __M_WEB_SERVER_H__
#define __M_WEB_SERVER_H__


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>


// Header functions ============================================================

// Initialize the web server and the DNS server
void initWebServer(bool output, int max_clients, bool hide_ssid);


#endif // __M_WEB_SERVER_H__