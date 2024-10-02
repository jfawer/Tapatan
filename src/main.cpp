#include <Arduino.h>
#define onboardLED 13

int number = 0;


void setup() {
  pinMode(onboardLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(onboardLED, HIGH);
  delay(1000);
  digitalWrite(onboardLED, LOW);
  delay(1000);
  number++;
  Serial.println("Durchlauf: " + String(number));
}