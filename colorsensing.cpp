#include <Arduino.h>


#define LED_R 2
#define LED_G 4
#define LED_B 3
#define LED_IR 4

#define PHOTOTRANSISTOR_Visible A5
#define PHOTOTRANSISTOR_IR 6


#define COLOR_THRESHOLD 50
#define distanceCalubrationFactor 1
void initializeColorSensing();
char currentColor();
int readColorValue(int ledPin, int photoPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initializeColorSensing();
  Serial.println("Color Sensing Initialized");
  delay(500);
}

void loop() {
    currentColor();
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
  int irValue = readColorValue(LED_IR, PHOTOTRANSISTOR_IR);
  return irValue * distanceCalubrationFactor;
}



