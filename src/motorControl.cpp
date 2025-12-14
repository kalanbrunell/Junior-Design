#include "motorControl.h"
#include <Arduino.h>

/*
 * name:      MotorControl::initializeMotion(int pin0, int pin1, int pin2, int pin3, float trim1, float trim2)
 * purpose:   Initialize motor control pins and trim factors
 * arguments: pin0 - PWM pin for motor 1 forward
 *            pin1 - PWM pin for motor 1 backward
 *            pin2 - PWM pin for motor 2 backward
 *            pin3 - PWM pin for motor 2 forward
 *            trim1 - trim factor for motor 1
 *            trim2 - trim factor for motor 2
 * returns:   None
 * effects:   Sets up the motor control pins as outputs and initializes them to HIGH
 * other:
 */
void MotorControl::initializeMotion(int pin0, int pin1, int pin2, int pin3, float trim1, float trim2) {
    this->_pin0 = pin0;
    this->_pin1 = pin1;
    this->_pin2 = pin2;
    this->_pin3 = pin3;
    this->_trim1 = trim1;
    this->_trim2 = trim2;

    pinMode(_pin0, OUTPUT);
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pin3, OUTPUT);

    digitalWrite(_pin0, HIGH);
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, HIGH);
    digitalWrite(_pin3, HIGH);
}

/*
 * name:      MotorControl::setSpeed(int motor1, int motor2)
 * purpose:   Set the speed of the motors
 * arguments: motor1 - speed for motor 1 (-255 to 255)
 *            motor2 - speed for motor 2 (-255 to 255)
 * returns:   None
 * effects:   Sets the speed of the motors based on the provided values and trim factors
 * other:
 */
void MotorControl::setSpeed(int motor1, int motor2) {
    motor1 = (int)(motor1 * _trim1);
    motor2 = (int)(motor2 * _trim2);
    if (abs(motor1) > 255 || abs(motor2) > 255) {
        Serial.println("ERROR: Max absolute speed value is 255");
        return;
    }
    //Serial.print(motor1);
    //Serial.print(", ");
    //Serial.println(motor2);
    if (motor1 > 0) {
        //Serial.println("m1positive");
        analogWrite(_pin0, motor1);
        analogWrite(_pin1, 0);
    } else {
        //Serial.println("m1negative");
        analogWrite(_pin1, -motor1);
        analogWrite(_pin0, 0);
    }
    if (motor2 < 0) {
        //Serial.println("m2negative");
        analogWrite(_pin2, -motor2);
        analogWrite(_pin3, 0);
    } else {
        //Serial.println("m2positive");
        analogWrite(_pin3, motor2);
        analogWrite(_pin2, 0);
    }
}

