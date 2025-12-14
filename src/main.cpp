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


// WiFi and WebSocket client setup
WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, SERVERADDRESS, PORT);

// Global instances
MotorControl motorControl;
ColorSensing colorSensing;
DistanceSensing distanceSensing;

//Variables
int status = WL_IDLE_STATUS;
int count = 0;

String receivedMessage = "";
int messageLength = 0;

char currentColorChar = '\0';

char followColorChar = 'N';

int BOTID = 1; //Set Bot ID here


// Local inter-class function declarations
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

// SOLO DEMO CODE-------------------------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------------------------------------------

//PARTNER DEMO CODE----------------------------------------------------------------------------------------------

    // Serial.println("loop start");
    ws::init(client, CLIENTID);
    while (client.connected()) {
        String EXT = "1";
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
        }

        while(digitalRead(BUTTON_PIN) != LOW) {
            indicateSolid('W');
        }; //Wait for button press to start demo
        //BOT1
        if (BOTID == 1) {
            ws::sendMessage(client, String("B1:START"));
            indicateFlash('B');
            driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
            turnRight(motorControl, 180); //180 deg right turn  
            driveStraightUntilColor(colorSensing, motorControl, 'R'); //straight until red
            turnLeft(motorControl, 90); //90 deg left turn
            ws::sendMessage(client, String("B1:AT_RED")); //Other bot should start 
            indicateFlash('B');
            followLine_L('R', 60, 15, colorSensing, motorControl, distanceSensing); //follow red
            turnLeft(motorControl, 90); //90 deg left turn
            driveStraightUntilColor(colorSensing, motorControl, 'Y'); //straight until yellow
            turnLeft(motorControl, 90); //90 deg left turn
            //Wait for signal from other bot to proceed
            //poll until message received
            while(true) {
                receivedMessage = ws::poll(client);
                //messageLength = receivedMessage.length();
                if (receivedMessage == EXT + "B2:AT_BLUE") {
                    indicateFlash('G');
                    Serial.println("Received AT_BLUE from other bot, proceeding along yellow");
                    ws::sendMessage(client, String("B1:ACK_AT_BLUE"));
                    break;
                }
            }
            followLine_R('Y', 60, 15, colorSensing, motorControl, distanceSensing); //follow yellow
            turnRight(motorControl, 90); //90    deg left turn
            driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
            indicateFlash('G');
            ws::sendMessage(client, String("B1:HOME"));
            while(true) {
                receivedMessage = ws::poll(client);
                //messageLength = receivedMessage.length();
                if (receivedMessage == EXT + "B2:HOME") {
                    indicateFlash('G');
                    Serial.println("Received AT_BLUE from other bot, proceeding along yellow");
                    ws::sendMessage(client, String("B1:ACK_HOME"));
                    break;
                }
            }
        } else {
            //BOT2
            //Wait for signal from other bot to start
            while(true) {
                receivedMessage = ws::poll(client);
                if (receivedMessage == EXT + "B1:AT_RED") {
                    indicateFlash('G');
                    Serial.println("Received AT_RED from other bot, commencing");
                    ws::sendMessage(client, String("B2:START"));
                    break;
                }
            }
            driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
            turnLeft(motorControl, 180); //180 deg left turn
            driveStraightUntilColor(colorSensing, motorControl, 'B'); //straight until blue
            turnRight(motorControl, 90); //90 deg right turn
            indicateFlash('B');
            ws::sendMessage(client, String("B2:AT_BLUE")); //Notify other bot at blue
            followLine_R('B', 60, 15, colorSensing, motorControl, distanceSensing); //follow blue
            //wait until other bot is moving
            while(true) {
                receivedMessage = ws::poll(client);
                if (receivedMessage == EXT + "B1:ACK_AT_BLUE") {
                    indicateFlash('G');
                    Serial.println("Received ACK_AT_BLUE from other bot, proceeding to yellow start");
                    break;
                }
            }
            turnRight(motorControl, 90); //90 deg right turn
            driveStraightUntilColor(colorSensing, motorControl, 'Y'); //straight until yellow
            turnRight(motorControl, 90); //90 deg right turn
            while(true) {
                receivedMessage = ws::poll(client);
                if (receivedMessage == EXT + "B1:HOME") {
                    indicateFlash('G');
                    Serial.println("Received HOME from other bot, proceeding along yellow");
                    break;
                }
            }
            followLine_R('Y', 60, 15, colorSensing, motorControl, distanceSensing); //follow yellow
            turnRight(motorControl, 90); //90    deg left turn
            driveStraighUntilStop(motorControl, distanceSensing); //straight until stop
            ws::sendMessage(client, String("B2:HOME")); //Notify other bot has returned
            indicateFlash('G');
        }
        
        
//END PARTNER DEMO CODE---------------------------------------------------------------------------------------

//Partner team Remote Bot Motion Milestone Code --------------------------------------------------------------
        /*String ext = "1";
        if (BOTID == 1) {
            //Bot1
            indicateFlash('B');
            ws::sendMessage(client, String("team2:START"));
            while(1) {
                receivedMessage = ws::poll(client);
                if (receivedMessage.length() > 0) {
                    Serial.println("Message Received: " + receivedMessage);
                    if (receivedMessage == ext + "team1:START") {
                        Serial.println(" -- Commencing Movement");
                        indicateFlash('G');
                        delay(500);
                        motorControl.setSpeed(100, 100);
                        delay(5000);
                        motorControl.setSpeed(0, 0);
                        break;
                    }
                }*/
                //Serial.print("Message Received: " + receivedMessage);
                // if (receivedMessage.startsWith("F392FC86D8D7")) {
                //     Serial.println(" -- Commencing Movement");
                //     indicateFlash('G');
                //     delay(500);
                //     motorControl.setSpeed(100, 100);
                //     delay(5000);
                //     motorControl.setSpeed(0, 0);
                //     return;
                // }
            //}
        //}
//---------------------------------------------------------------------------------------------------------------
    }
}


/*
 * name:      followLine_L(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense)
 * purpose:   Follow the left edge of a specific color.
 * arguments: colorChar - character representing the target color to follow
 *            baseSpeed - the base speed for both motors
 *            tuningConst - the constant used to adjust motor speeds for line following
 *            colorsense - instance of ColorSensing class for color detection
 *            motors - instance of MotorControl class for motor control
 *            distancesense - instance of DistanceSensing class for distance measurement
 * returns:   None
 * effects:   Continuously adjusts motor speeds to follow the left edge of the specified color 
                until an obstacle is detected
 * other:
 */
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

/*
 * name:      followLine_R(char colorChar, int baseSpeed, int tuningConst, ColorSensing colorsense, MotorControl motors, DistanceSensing distancesense)
 * purpose:   Follow the left edge of a specific color.
 * arguments: colorChar - character representing the target color to follow
 *            baseSpeed - the base speed for both motors
 *            tuningConst - the constant used to adjust motor speeds for line following
 *            colorsense - instance of ColorSensing class for color detection
 *            motors - instance of MotorControl class for motor control
 *            distancesense - instance of DistanceSensing class for distance measurement
 * returns:   None
 * effects:   Continuously adjusts motor speeds to follow the right edge of the specified color 
                until an obstacle is detected
 * other:
 */
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

/*
 * name:      turnRight(MotorControl motors, int degree)
 * purpose:   Turn the robot right by a specified degree.
 * arguments: motors - instance of MotorControl class for motor control
 *            degree - the degree to turn right
 * returns:   None
 * effects:   Turns the robot right by adjusting motor speeds and delaying for a calculated time
 * other:
 */
void turnRight(MotorControl motors, int degree) {
    motors.setSpeed(60, -60);
    delay(degree * 9); //adjust delay for turn
    motors.setSpeed(0, 0);
}

/*
 * name:      turnLeft(MotorControl motors, int degree)
 * purpose:   Turn the robot left by a specified degree.
 * arguments: motors - instance of MotorControl class for motor control
 *            degree - the degree to turn left
 * returns:   None
 * effects:   Turns the robot left by adjusting motor speeds and delaying for a calculated time
 * other:
 */
void turnLeft(MotorControl motors, int degree) {
    motors.setSpeed(-60, 60);
    delay(degree * 9); //adjust delay for turn
    motors.setSpeed(0, 0);
}

/*
 * name:      driveStraightUntilColor(ColorSensing colorsense, MotorControl motors, char colorChar)
 * purpose:   Drive straight until a specific color is detected.
 * arguments: colorsense - instance of ColorSensing class for color detection
 *            motors - instance of MotorControl class for motor control
 *            colorChar - character representing the target color to detect
 * returns:   None
 * effects:   Drives the robot straight until the specified color is detected consistently
 * other:
 */
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

/*
 * name:      driveStraighUntilStop(MotorControl motors, DistanceSensing distancesense)
 * purpose:   Drive straight until an obstacle is detected.
 * arguments: motors - instance of MotorControl class for motor control
 *            distancesense - instance of DistanceSensing class for distance measurement
 * returns:   None
 * effects:   Drives the robot straight until an obstacle is detected consistently
 * other:
 */
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

/*
 * name:      indicateSolid(char colorChar)
 * purpose:   Indicate a solid color on the RGB LED.
 * arguments: colorChar - character representing the color to display ('R', 'G', 'B', 'Y', 'N')
 * returns:   None
 * effects:   Sets the RGB LED to display the specified solid color
 * other:
 */
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
    } else if(colorChar == 'W') {
        digitalWrite(INDC_R_PIN, HIGH);
        digitalWrite(INDC_G_PIN, HIGH);
        digitalWrite(INDC_B_PIN, HIGH);
    } else {
        digitalWrite(INDC_R_PIN, LOW);
        digitalWrite(INDC_G_PIN, LOW);
        digitalWrite(INDC_B_PIN, LOW);
    }
}

/*
 * name:      indicateFlash(char colorChar)
 * purpose:   Indicate a flashing color on the RGB LED.
 * arguments: colorChar - character representing the color to display ('R', 'G', 'B', 'Y', 'N')
 * returns:   None
 * effects:   Flashes the RGB LED with the specified color twice
 * other:
 */
void indicateFlash(char colorChar){
    for(int i = 0; i < 2; i++) {
        indicateSolid(colorChar);
        delay(250);
        indicateSolid('N');
        delay(250);
    }
}