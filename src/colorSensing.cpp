#include "colorSensing.h"
#include <Arduino.h>

/*
 * name:      ColorSensing::initializeColorSensing(int ledR, int ledG, int ledB, int ledIR,
                                    int photoVisible, int photoIR)
 * purpose:   Initialize color sensing pins
 * arguments: ledR - pin for red LED
              ledG - pin for green LED
              ledB - pin for blue LED
              ledIR - pin for IR LED
              photoVisible - pin for visible light phototransistor
              photoIR - pin for IR phototransistor
 * returns:   None
 * effects:   Sets up the color sensing pins for use
 * other:
 */
void ColorSensing::initializeColorSensing(int ledR, int ledG, int ledB, int ledIR,
                                    int photoVisible, int photoIR) {
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledIR, OUTPUT);

  pinMode(photoVisible, INPUT);
  pinMode(photoIR, INPUT);
}


/*
 * name:      ColorSensing::readColorValue(int ledPin, int photoPin)
 * purpose:   Read the color value using specified LED and phototransistor pins
 * arguments: ledPin - pin for the LED to illuminate
              photoPin - pin for the phototransistor to read
 * returns:   Measured color value
 * effects:   Reads the color value by illuminating the LED and measuring the phototransistor response
 * other:
 */
int ColorSensing::readColorValue(int ledPin, int photoPin){
  digitalWrite(ledPin, HIGH);
  delay(5); // Allow sensor to stabilize
  int value = analogRead(photoPin);
  digitalWrite(ledPin, LOW);

  return value;
}


/*
 * name:      ColorSensing::currentColor()
 * purpose:   Determine the current color detected by the sensor
 * arguments: None
 * returns:   Character representing the detected color ('R', 'B', 'Y', 'N')
 * effects:   Reads color values and determines the current color based on thresholds
 * other:
 */
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

  if(redValue > 250) {
    //Serial.println("Yellow Color Detected");
    return 'Y';
  } else if(blueValue > 300) {
    //Serial.println("Blue Color Detected");
    return 'B';
  }else if(redValue >= 100 && blueValue > 40) {
    //Serial.println("Red Color Detected");
    return 'R';
  } else {
    //Serial.println("Black Color Detected");
    return 'N';
  }
}