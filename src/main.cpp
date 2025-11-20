#include "motorControl.h"
#include "colorSensing.h"
#include "config.h"
#include "webSocket.h"
#include "distanceSensing.h"

#include <Arduino.h>
#include <assert.h>
#include <stdio.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>


WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, SERVERADDRESS, PORT);

// Global instances
MotorControl motorControl;
ColorSensing colorSensing;
DistanceSensing distanceSensing;

int status = WL_IDLE_STATUS;
int count = 0;

String receivedMessage = "";
int messageLength = 0;

char currentColorChar = '\0';

char followColorChar = 'B';

int motor1Speed = 60;
int motor2Speed = 60;

int tuning_constant = 25;

// Minimal ISR stub for button interrupt
void ISR_button_pressed() {}

void setup() {
    Serial.begin(9600);
    motorControl.initializeMotion(pin0, pin1, pin2, pin3, TRIM1, TRIM2);
    colorSensing.initializeColorSensing(LED_R, LED_G, LED_B, LED_IR, 
                                    PHOTOTRANSISTOR_Visible, PHOTOTRANSISTOR_IR);
    distanceSensing.initializeDistanceSensing();
    pinMode(12, INPUT_PULLUP); //for button
    pinMode(13, OUTPUT); //for attached LED

    while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SSID);  // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SSID, PASS);
    }

    ws::printWifiDiagnostics();

    attachInterrupt(digitalPinToInterrupt(12), ISR_button_pressed, FALLING);
    colorSensing.initializeColorSensing(LED_R, LED_G, LED_B, LED_IR, 
                                    PHOTOTRANSISTOR_Visible, PHOTOTRANSISTOR_IR);
    
}

void loop() {
    while (true)
    {   
        
        delay(25);
        currentColorChar = colorSensing.currentColor();
        //Serial.println("Current Color Detected: ");
        //Serial.println(currentColorChar);
        //Serial.println("----------------------");
    
        if (currentColorChar == followColorChar) {
            //Serial.println("Following Color Detected");
            motor1Speed = motor1Speed - tuning_constant;
            motor2Speed= motor2Speed + tuning_constant;
        } else {
            motor1Speed = motor1Speed + tuning_constant;
            motor2Speed = motor2Speed - tuning_constant;
        }

        if(motor1Speed <  55) motor1Speed = 55;
        if(motor2Speed < 55) motor2Speed = 55;
        if(motor1Speed > 100) motor1Speed = 100;
        if(motor2Speed > 100) motor2Speed = 100;

        

        if(distanceSensing.readIRValue() < -25) {
            Serial.println("Motors Stopped");
            motor1Speed = 0;
            motor2Speed = 0;
        }

        motorControl.setSpeed(motor1Speed, motor2Speed);
    }
    
    ws::init(client, CLIENTID);
    while (client.connected()) {
        
        //Poll first to find a message from Mac and Cheese team
        receivedMessage = ws::poll(client);
        messageLength = receivedMessage.length();
        if (messageLength > 0) { 
            Serial.println("Message Received: " + receivedMessage); //Mac and Cheese Team Command Received
            if(receivedMessage.charAt(0) == '0') {
                Serial.println("Internal Control Mode");
            } else if(receivedMessage.charAt(0) == '1') {
                Serial.println("External Control Mode: Commencing");
            }
            // do something with this message... WIP
        }
        //other team wants state 7
        currentColorChar = colorSensing.currentColor();
        if (currentColorChar == followColorChar) {
            motorControl.setSpeed(100, 75);
        } else {
            motorControl.setSpeed(75, 100);
        }
    }
}