#include <Arduino.h>
#include <assert.h>


#define pin0 10
#define pin1 9
#define pin2 5
#define pin3 6
#define TRIM1 .935
#define TRIM2 1

#define LED_R 2
#define LED_B 3
#define LED_G 4
#define LED_IR 7

#define PHOTOTRANSISTOR_Visible A5
#define PHOTOTRANSISTOR_IR A4


#define COLOR_THRESHOLD 50
#define distanceCalubrationFactor 1
void initializeColorSensing();
char currentColor();
int currentDistance();
bool stop();
int readIRValue(int ledPin, int photoPin);
int readColorValue(int ledPin, int photoPin);
void setSpeed(int motor1, int motor2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initializeColorSensing();
  Serial.println("Color Sensing Initialized");
  delay(500);
}

void loop() {
  if (stop()) {
    setSpeed(0, 0);
    Serial.println("STOP");
    // while (stop());
  } else {
    setSpeed(100, 100);
    Serial.println("GO");
  }
}

// put function definitions here:

void initializeColorSensing() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_IR, OUTPUT);

  pinMode(PHOTOTRANSISTOR_Visible, INPUT);
  pinMode(PHOTOTRANSISTOR_IR, INPUT);
}

int readColorValue(int ledPin, int photoPin){
  digitalWrite(ledPin, HIGH);
  delay(10);
  int value = analogRead(photoPin);
  digitalWrite(ledPin, LOW);

  return value;
}

int readIRValue(int ledPin, int photoPin){
  digitalWrite(ledPin, LOW);
  delay(10);
  int ambient = analogRead(photoPin);
  digitalWrite(ledPin, HIGH);
  delay(10);
  int value = analogRead(photoPin);

  return value - ambient;
}

char currentColor() {
  int redValue = readColorValue(LED_R, PHOTOTRANSISTOR_Visible);
  int greenValue = readColorValue(LED_G, PHOTOTRANSISTOR_Visible);
  int blueValue =  readColorValue(LED_B, PHOTOTRANSISTOR_Visible);
  int sum = redValue + greenValue + blueValue;



  if (sum < 5) {
    Serial.println("Black Color Detected");
    //return 'N';
  } else if(blueValue > 30 && greenValue != 0){
    Serial.println("Yellow Color Detected");
    //return 'Y';
  } else if(blueValue > redValue && blueValue > greenValue) {
    Serial.println("Blue Color Detected");
    //return 'B';
  } else if(redValue > greenValue && redValue > blueValue) {
    Serial.println("Red Color Detected");
    //return 'R';
  }

  
}

int currentDistance() {
  return readIRValue(LED_IR, PHOTOTRANSISTOR_IR);
}


bool stop() {
  return (currentDistance() < -20);;
}

void setSpeed(int motor1, int motor2) {
  motor1 *= TRIM1;
  motor2 *= TRIM2;
  if (abs(motor1) > 255 || abs(motor2) > 255) {
    Serial.println("ERROR: Max absolute speed value is 255");
    assert(false);
  }
  // Serial.print(motor1);
  // Serial.print(motor2);
  if (motor1 > 0) {
    // Serial.println("m1positive");
    analogWrite(pin0, motor1);
    analogWrite(pin1, 0);
  } else {
    // Serial.println("m1negative");
    analogWrite(pin1, -motor1);
    analogWrite(pin0, 0);
  }
  if (motor2 < 0) {
    // Serial.println("positive");
    analogWrite(pin2, -motor2);
    analogWrite(pin3, 0);
  } else {
    // Serial.println("negative");
    analogWrite(pin3, motor2);
    analogWrite(pin2, 0);
  }
}