#ifndef motorControl_h
#define motorControl_h

#include <Arduino.h>
#include "config.h"

class MotorControl {
    public:
        void setSpeed(int motor1, int motor2);
        void initializeMotion(int pin0, int pin1, int pin2, int pin3, float trim1, float trim2);
    private:
        int _pin0, _pin1, _pin2, _pin3;
        float _trim1, _trim2;
};

#endif