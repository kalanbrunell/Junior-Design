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

char followColorChar = 'N';


// Minimal ISR stub for button interrupt
void ISR_button_pressed() {}
void indicateSolid(char colorChar);
void indicateFlash(char colorChar);
void followLine_L(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense);
void followLine_R(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense);
void turnRight(MotorControl motors, int degree);
void turnLeft(MotorControl motors, int degree);
void driveStraightUntilColor(ColorSensing colorsense, MotorControl motors, char colorChar);
void driveStraighUntilStop(MotorControl motors, DistanceSensing distancesense);

void setup() {
    pinMode(INDC_B_PIN, OUTPUT);
    pinMode(INDC_G_PIN, OUTPUT);
    pinMode(INDC_R_PIN, OUTPUT);
    indicateFlash('B');
    indicateSolid('R');
    Serial.begin(9600);
    motorControl.initializeMotion(pin0, pin1, pin2, pin3, TRIM1, TRIM2);
    colorSensing.initializeColorSensing(LED_R, LED_G, LED_B, LED_IR, 
                                    PHOTOTRANSISTOR_Visible, PHOTOTRANSISTOR_IR);
    distanceSensing.initializeDistanceSensing();
    pinMode(BUTTON_PIN, INPUT_PULLUP); //for button
    pinMode(13, OUTPUT); //for attached LED
    pinMode(VBAT_PIN, INPUT); //for battery voltage reading
    
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
    indicateSolid('B');
    /*
    while(true) {
        Serial.println("VBAT: " + String((float(analogRead(VBAT_PIN)) * VBAT_SCALE) * (5.0 / 1023.0)) + " V");
        delay(500);
    }
        */
    // while(true) {
        
    //     while(digitalRead(BUTTON_PIN) != LOW) {}
    //     //Bot1
    //     driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
    //     turnRight(motorControl, 180); //180 deg right turn  
    //     driveStraightUntilColor(colorSensing, motorControl, 'R'); //straight until red
    //     turnLeft(motorControl, 90); //90 deg left turn
    //     followLine_L('R', 60, 15, colorSensing, motorControl, distanceSensing); //follow red
    //     turnLeft(motorControl, 90); //90 deg left turn
    //     driveStraightUntilColor(colorSensing, motorControl, 'Y'); //straight until yellow
    //     turnLeft(motorControl, 90); //90 deg left turn
    //     followLine_L('Y', 60, 15, colorSensing, motorControl, distanceSensing); //follow yellow
    //     turnLeft(motorControl, 90); //90    deg left turn
    //     driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
        
    //     while(digitalRead(BUTTON_PIN) != LOW) {}
    //     //Bot2
    //     driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
    //     turnLeft(motorControl, 180); //180 deg left turn  
    //     driveStraightUntilColor(colorSensing, motorControl, 'B'); //straight until blue
    //     turnRight(motorControl, 90); //90 deg right turn
    //     driveStraightUntilColor(colorSensing, motorControl, 'B'); //straight until blue
    //     followLine_R('B', 60, 15, colorSensing, motorControl, distanceSensing); //follow blue
    //     turnRight(motorControl, 90); //90 deg right turn
    //     driveStraightUntilColor(colorSensing, motorControl, 'Y'); //straight until yellow
    //     turnRight(motorControl, 90); //90 deg right turn
    //     followLine_R('Y', 60, 15, colorSensing, motorControl, distanceSensing); //follow yellow
    //     turnRight(motorControl, 90); //90    deg left turn
    //     driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
    // }

    // Serial.println("loop start");
    ws::init(client, CLIENTID);
    while (client.connected()) {
        indicateSolid('G');
        //Poll first to find a message from Mac and Cheese team
        receivedMessage = ws::poll(client);
        messageLength = receivedMessage.length();
        if (messageLength > 0) { 
            Serial.print("Message Received: " + receivedMessage); //Mac and Cheese Team Command Received
            if(receivedMessage.charAt(0) == '0') {
                Serial.println(" -- Internal Control Mode");
            } else if(receivedMessage.charAt(0) == '1') {
                Serial.println(" -- External Control Mode: Commencing");
            } else {
                Serial.println("");
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

void followLine_L(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense) {
    int motor1Speed = baseSpeed;
    int motor2Speed = baseSpeed;

    while (true) {   
        delay(25);
        char currentColor = colorsense.currentColor();
        if (currentColor == colorChar) {
            motor1Speed = motor1Speed - tuningConst;
            motor2Speed= motor2Speed + tuningConst;
        } else {
            motor1Speed = motor1Speed + tuningConst;
            motor2Speed = motor2Speed - tuningConst;
        }

        if(motor1Speed <  55) motor1Speed = 55;
        if(motor2Speed < 55) motor2Speed = 55;
        if(motor1Speed > 100) motor1Speed = 100;
        if(motor2Speed > 100) motor2Speed = 100;

        motors.setSpeed(motor1Speed, motor2Speed);

        if(distancesense.readIRValue() < -25) {
            motors.setSpeed(0, 0);
            return;
        }
        
    }
    

}

void followLine_R(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense) {
    int motor1Speed = baseSpeed;
    int motor2Speed = baseSpeed;

    while (true) {   
        delay(25);
        char currentColor = colorsense.currentColor();
        if (currentColor != colorChar) {
            motor1Speed = motor1Speed - tuningConst;
            motor2Speed= motor2Speed + tuningConst;
        } else {
            motor1Speed = motor1Speed + tuningConst;
            motor2Speed = motor2Speed - tuningConst;
        }

        if(motor1Speed <  55) motor1Speed = 55;
        if(motor2Speed < 55) motor2Speed = 55;
        if(motor1Speed > 100) motor1Speed = 100;
        if(motor2Speed > 100) motor2Speed = 100;

        motors.setSpeed(motor1Speed, motor2Speed);

        if(distancesense.readIRValue() < -25) {
            motors.setSpeed(0, 0);
            return;
        }
        
    }
    

}

void turnRight(MotorControl motors, int degree) {
    motors.setSpeed(60, -60);
    delay(degree * 9); //adjust delay for turn
    motors.setSpeed(0, 0);
}
void turnLeft(MotorControl motors, int degree) {
    motors.setSpeed(-60, 60);
    delay(degree * 9); //adjust delay for turn
    motors.setSpeed(0, 0);
}
void driveStraightUntilColor(ColorSensing colorsense, MotorControl motors, char colorChar) {
    motors.setSpeed(80, 80);
    int targetCounter = 0;
    while(true) {
        delay(25);
        char currentColor = colorsense.currentColor();
        if(currentColor == colorChar) {
            targetCounter++;
        } else if (currentColor != colorChar && targetCounter > 0) {
            targetCounter = 0;
        }
        if(targetCounter >= 3) {
            motors.setSpeed(0, 0);
            return;
        }
    }
}
void driveStraighUntilStop(MotorControl motors, DistanceSensing distancesense) {
    motors.setSpeed(80, 80);
    while(true) {
        delay(25);
        int dist = distancesense.readIRValue();
        int value = (dist < -500) ? value + 1 : 0;
        // Serial.println(dist);
        if(value > 3) {
            value = 0;
            motors.setSpeed(0, 0);
            return;
        }
    }
}

void indicateSolid(char colorChar) {
    if(colorChar == 'R') {
        digitalWrite(INDC_R_PIN, HIGH);
        digitalWrite(INDC_G_PIN, LOW);
        digitalWrite(INDC_B_PIN, LOW);
    } else if(colorChar == 'G') {
        digitalWrite(INDC_R_PIN, LOW);
        digitalWrite(INDC_G_PIN, HIGH);
        digitalWrite(INDC_B_PIN, LOW);
    } else if(colorChar == 'B') {
        digitalWrite(INDC_R_PIN, LOW);
        digitalWrite(INDC_G_PIN, LOW);
        digitalWrite(INDC_B_PIN, HIGH);
    } else if(colorChar == 'Y') {
        digitalWrite(INDC_R_PIN, HIGH);
        digitalWrite(INDC_G_PIN, HIGH);
        digitalWrite(INDC_B_PIN, LOW);
    } else {
        digitalWrite(INDC_R_PIN, LOW);
        digitalWrite(INDC_G_PIN, LOW);
        digitalWrite(INDC_B_PIN, LOW);
    }
}
void indicateFlash(char colorChar){
    for(int i = 0; i < 2; i++) {
        indicateSolid(colorChar);
        delay(250);
        indicateSolid('N');
        delay(250);
    }
}