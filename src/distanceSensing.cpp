#include "distanceSensing.h"
#include <Arduino.h>

void DistanceSensing::initializeDistanceSensing() {
    pinMode(LED_IR, OUTPUT);
    pinMode(PHOTOTRANSISTOR_IR, INPUT);
}

int DistanceSensing::readIRValue(){
    digitalWrite(LED_IR, LOW);
    delay(10);
    int ambient = analogRead(PHOTOTRANSISTOR_IR);
    digitalWrite(LED_IR, HIGH);
    delay(10);
    int value = analogRead(PHOTOTRANSISTOR_IR);

    return (value - ambient) * max(1.0, ambient) * 0.01;
}
