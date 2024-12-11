// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "MotorController.h"

MotorController motorController(23, 3, 4, 5, 6, 7, 8, 17, 10, 11);

// Konstanten für die Motorsteuerung
MotorConfig config;

void setup() {
  Serial.begin(9600);

  
  // Initialisierung der Motorsteuerung
  motorController.initialize();
  motorController.setConfig(config);
  
  /*
  delay(2000);

  // Motoren Nullen
  motorController.homeMotors();
  delay(3000);
  
  // An einen Punkt fahren
  motorController.moveToPosition(100, 150);
  delay(3000);
  
  // Einen Stein bewegen
  Move move = {20, 50, 140, 200};
  motorController.moveStone(move);
  delay(3000);
  */

  /*
  motorController.turnElectromagnetOn();
  delay(2000);
  motorController.turnElectromagnetOff();
  delay(2000);
  motorController.setElectromagnetPolarityPositive();
  delay(2000);
  motorController.setElectromagnetPolarityNegative();
  delay(2000);
  */

}

void loop() {

}