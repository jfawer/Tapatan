// Einbinden der Bibliotheken
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

// Einbinden der Header-Dateien
#include "MotorController.h"

MotorController motorController(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 315, 405, 0, 0);

void setup() {
  Serial.begin(9600);
  motorController.initialize();
  motorController.homeMotors();
}

void loop() {
  motorController.enableMotors();
  motorController.moveToPosition(150, 200);
  delay(2000);
  motorController.moveToPosition(100, 100);
  motorController.disableMotors();
  delay(5000);
}