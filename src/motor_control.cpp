// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "MotorController.h"

MotorController motorController(2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

// Konstanten für die Motorsteuerung
MotorConfig config;

void setup() {
  Serial.begin(9600);

  // Initialisierung der Motorsteuerung
  motorController.initialize();
  motorController.setConfig(config);

  // Test der Motorsteuerung
  motorController.enableMotors();
  delay(3000);

  // An einen Punkt fahren
  motorController.moveToPosition(150, 200);
  delay(3000);

  // Motoren Nullen
  motorController.homeMotors();
  delay(3000);

  // Einen Stein bewegen
  Move move = {20, 50, 140, 200};
  motorController.moveStone(move);
  delay(3000);

  motorController.disableMotors();
}

void loop() {
  Serial.println("Loop");
  delay(3000);
}