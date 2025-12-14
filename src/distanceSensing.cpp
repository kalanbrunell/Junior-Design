#include "distanceSensing.h"
#include <Arduino.h>

/*
 * name:      DistanceSensing::initializeDistanceSensing()
 * purpose:   Initialize distance sensing pins
 * arguments: None
 * returns:   None
 * effects:   Sets up the distance sensing pins for use
 * other:
 */
void DistanceSensing::initializeDistanceSensing() {
    pinMode(LED_IR, OUTPUT);
    pinMode(PHOTOTRANSISTOR_IR, INPUT);
}

/*
 * name:      DistanceSensing::readIRValue()
 * purpose:   Read the IR distance value
 * arguments: None
 * returns:   None
 * effects:   Reads the IR value from the phototransistor and returns the processed distance value
 * other:
 */
int DistanceSensing::readIRValue(){
    digitalWrite(LED_IR, LOW);
    delay(10);
    int ambient = analogRead(PHOTOTRANSISTOR_IR);
    digitalWrite(LED_IR, HIGH);
    delay(10);
    int value = analogRead(PHOTOTRANSISTOR_IR);

    return (value - ambient) * max(1.0, ambient) * 0.01;
}
