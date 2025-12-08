#include "webSocket.h"
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "config.h"

void ws::init(WebSocketClient &client, const char* clientID, const char* path) {
    Serial.println("starting WebSocket client");
    client.begin(path);
    Serial.println("WebSocket client started");
    client.beginMessage(TYPE_TEXT);
    Serial.print("Sending Client ID: ");
    Serial.println(clientID);
    client.print(clientID);
    client.endMessage();
}

void ws::printWifiDiagnostics() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

String ws::poll(WebSocketClient &client) {
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
        String receivedMessage = client.readString();
        if (receivedMessage.startsWith(String(MACCHEESEFILTER))) {
            int dotIndex = receivedMessage.indexOf('.');
            Serial.println(dotIndex);
            if (dotIndex != -1) {
                return "0" + receivedMessage.substring(dotIndex + 1);
            }
        } else if (receivedMessage.startsWith(String(EXTERNALFILTER))) {
            int dotIndex = receivedMessage.indexOf('.');
            if (dotIndex != -1) {
                return "1" + receivedMessage.substring(dotIndex + 1);
            }
        }
        else return ("UNFILTERED: " + receivedMessage);
    }
    return "";
}