#ifndef webSocket_h
#define webSocket_h

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "config.h"

// Helpers for ArduinoHttpClient::WebSocketClient
namespace ws {
    void init(WebSocketClient &client, const char* clientID, const char* path = "/");
    void printWifiDiagnostics();
    String poll(WebSocketClient &client);
    void sendMessage(WebSocketClient &client, const String &message);
}

#endif