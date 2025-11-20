#include "colorSensing.h"
#include <Arduino.h>


void ColorSensing::initializeColorSensing(int ledR, int ledG, int ledB, int ledIR,
                                    int photoVisible, int photoIR) {
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledIR, OUTPUT);

  pinMode(photoVisible, INPUT);
  pinMode(photoIR, INPUT);
}

int ColorSensing::readColorValue(int ledPin, int photoPin){
  digitalWrite(ledPin, HIGH);
  delay(5); // Allow sensor to stabilize
  int value = analogRead(photoPin);
  digitalWrite(ledPin, LOW);

  return value;
}

char ColorSensing::currentColor() {

  int redValue = readColorValue(LED_R, PHOTOTRANSISTOR_Visible);
  //Serial.println("Red Value: ");
  //Serial.println(redValue);
  int greenValue = readColorValue(LED_G, PHOTOTRANSISTOR_Visible);
  //Serial.println("Green Value: ");
  //Serial.println(greenValue);
  int blueValue =  readColorValue(LED_B, PHOTOTRANSISTOR_Visible);
  //Serial.println("Blue Value: ");
  //Serial.println(blueValue);


//red > 30 and green > 3 -> yellow
//blue > 35 -> blue
//red > 16 -> red

  if(redValue > 30 && greenValue > 3) {
    //Serial.println("Yellow Color Detected");
    return 'Y';
  } else if(blueValue > 35) {
    //Serial.println("Blue Color Detected");
    return 'B';
  }else if(redValue >= 16) {
    //Serial.println("Red Color Detected");
    return 'R';
  } else {
    //Serial.println("Black Color Detected");
    return 'N';
  }
}