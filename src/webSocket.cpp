#include "webSocket.h"
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "config.h"

/*
 * name:      ws::init(WebSocketClient &client, const char* clientID, const char* path)
 * purpose:   Initialize the WebSocket client and send the client ID to the server
 * arguments: client - reference to the WebSocketClient instance
 *            clientID - the client ID to send to the server
 *            path - the WebSocket server path
 * returns:   none
 * effects:   Initializes the WebSocket client and sends the client ID
 * other:
 */
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

/*
 * name:      ws::printWifiDiagnostics()
 * purpose:   Print WiFi diagnostics information
 * arguments: none
 * returns:   none
 * effects:   Prints the SSID and IP address of the WiFi connection to the Serial monitor
 * other:
 */
void ws::printWifiDiagnostics() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

/*
 * name:      ws::poll(WebSocketClient &client)
 * purpose:   Poll the WebSocket client for incoming messages
 * arguments: client - reference to the WebSocketClient instance
 * returns:   String - the processed message received from the server
 * effects:   Reads messages from the WebSocket client, processes them based on predefined filters, 
                and returns the modified message
 * other:
 */
String ws::poll(WebSocketClient &client) {
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
        String receivedMessage = client.readString();
        Serial.println("Raw Message Received: " + receivedMessage);
        if (receivedMessage.startsWith(String(MACCHEESEFILTER))) {
            int dotIndex = receivedMessage.indexOf(',');
            Serial.println(dotIndex);
            if (dotIndex != -1) {
                return "0" + receivedMessage.substring(dotIndex + 1);
            }
        } else if (receivedMessage.startsWith(String(EXTERNALFILTER))) {
            int dotIndex = receivedMessage.indexOf(',');
            if (dotIndex != -1) {
                return "1" + receivedMessage.substring(dotIndex + 1);
            }
        }
        else return ("UNFILTERED: " + receivedMessage);
    }
    return "";
}


/*
 * name:      ws::sendMessage(WebSocketClient &client, const String &message)
 * purpose:   Send a message to the WebSocket server
 * arguments: client - reference to the WebSocketClient instance
 *            message - the message to send to the server
 * returns:   None
 * effects:   Sends the specified message to the WebSocket server
 * other:
 */
void ws::sendMessage(WebSocketClient &client, const String &message) {
    client.beginMessage(TYPE_TEXT);
    client.print(message);
    client.endMessage();
}