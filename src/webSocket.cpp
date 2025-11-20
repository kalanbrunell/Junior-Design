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
            String parsedMessage = '0' + receivedMessage.substring(strlen(MACCHEESEFILTER));
            return parsedMessage;
        } else if(receivedMessage.startsWith(String(EXTERNALFILTER))) {
            String parsedMessage = '1' + receivedMessage.substring(strlen(EXTERNALFILTER));
            return parsedMessage;
        }
    }
    return "";
}