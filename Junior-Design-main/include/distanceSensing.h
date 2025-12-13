#ifndef distanceSensing_h
#define distanceSensing_h

#include <Arduino.h>
#include "config.h"

class DistanceSensing {
    public:
    void initializeDistanceSensing();
    int readIRValue();

    private:
        
};


#endif