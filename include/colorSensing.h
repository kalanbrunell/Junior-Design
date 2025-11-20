#ifndef colorSensing_h
#define colorSensing_h

#include <Arduino.h>
#include "config.h"

class ColorSensing {
    public:
        void initializeColorSensing(int ledR, int ledG, int ledB, int ledIR,
                                    int photoVisible, int photoIR);
        char currentColor();
        int readColorValue(int ledPin, int photoPin);
        //int currentDistance();
        //bool stop();
        //int readIRValue(int ledPin, int photoPin);
        //void setSpeed(int motor1, int motor2);
};

#endif