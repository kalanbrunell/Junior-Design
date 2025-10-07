#include <Arduino.h>
#include <assert.h>
#include <stdio.h>


#define pin0 10
#define pin1 9
#define pin2 5
#define pin3 6
#define TRIM1 .935
#define TRIM2 1

// put function declarations here:
void setSpeed(int motor1, int motor2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 55; i < 256; i++) {
    setSpeed(i, i);
    delay(5);
  }
  for (int i = 255; i > 127; i--)
  {
    setSpeed(i, i);
    delay(5);
  }
  for (int i = 128; i > 55; i--)
  {
    setSpeed(i, i);
    delay(5);
  }  
  for (int i = -55; i > -128; i--)
  {
    setSpeed(i, i);
    delay(5);
  }
  // for (int i = -128; i < -55; i++)
  // {
  //   setSpeed(i, i);
  //   delay(5);
  // }
  for (int i = -128; i < -55; i++)
  {
    setSpeed(-128, i);
    delay(5);
  }
  for (int i = -128; i < -55; i++)
  {
    setSpeed(i, 0);
    delay(5);
  }

}

void setSpeed(int motor1, int motor2) {
  motor1 *= TRIM1;
  motor2 *= TRIM2;
  if (abs(motor1) > 255 || abs(motor2) > 255) {
    Serial.println("ERROR: Max absolute speed value is 255");
    assert(false);
  }
  Serial.print(motor1);
  Serial.print(motor2);
  if (motor1 > 0) {
    Serial.println("m1positive");
    analogWrite(pin0, motor1);
    analogWrite(pin1, 0);
  } else {
    Serial.println("m1negative");
    analogWrite(pin1, -motor1);
    analogWrite(pin0, 0);
  }
  if (motor2 < 0) {
    Serial.println("positive");
    analogWrite(pin2, -motor2);
    analogWrite(pin3, 0);
  } else {
    Serial.println("negative");
    analogWrite(pin3, motor2);
    analogWrite(pin2, 0);
  }
}