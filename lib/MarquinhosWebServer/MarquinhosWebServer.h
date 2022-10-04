#ifndef __M_WEB_SERVER_H__
#define __M_WEB_SERVER_H__


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>


// Header functions ============================================================

// Initialize the web server and the DNS server
void initWebServer(bool output, int max_clients, bool hide_ssid);


// Extern variables ============================================================

extern DNSServer dnsServer;


#endif // __M_WEB_SERVER_H__