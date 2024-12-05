// Einbinden der Bibliotheken
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "display.h"
#include "input.h"
#include "tictactoe.h"
#include "tapatan.h"
#include "LED.h"
#include "motor.h"

// Maximale und Minimale Positionen in mm
const int maxXPosition = 315;
const int maxYPosition = 405;
const int minXPosition = 0;
const int minYPosition = 0;

int playerGaragePosition[5][2] = {                                                                                  // Positionen der Garagen des Spielers
  {20, 350}, 
  {80, 350}, 
  {140, 350}, 
  {200, 350},
  {260, 350}
};

int computerGaragePosition[5][2] = {                                                                                // Positionen der Garagen des Computers
  {20, 50}, 
  {80, 50},
  {140, 50}, 
  {200, 50}, 
  {260, 50}
};                                                                                                                  

int BoardPosition[3][3][2] = {                                                                                      // Positionen der Spielfelder
  {{80, 100}, {80, 150}, {80, 200}}, 
  {{140, 100}, {140, 150}, {140, 200}}, 
  {{200, 100}, {200, 150}, {200, 200}}
};

int horizontalLanePositions[2] = {120, 180};                                                                         // Array für die horizontalen Fahrbahnen (X-Achsen-Positionen)
int verticalLanePositions[2] = {75, 225};                                                                            // Array für die vertikalen Fahrbahnen (Y-Achsen-Positionen)


int playerGarage[5] = {0, 0, 0, 0, 0};                                                                              // Status der Garagen des Spielers
int computerGarage[5] = {1, 0, 1, 0, 0};                                                                            // Status der Garagen des Computers

int Board [3][3] = {{0,0,2},{0,2,2},{0,1,0}};                                                                       // Spielfeld
int BoardMemory [3][3] = {{0,2,1},{1,2,2},{0,1,0}};                                                                 // Speicherfeld

// Aktuelle Positionen in mm
int currentXPosition = 0;
int currentYPosition = 0;

// Pins für die Motoren
// Motor 1
const int motor1StepPin = 2;
const int motor1DirPin = 3;
const int motor1EnablePin = 4;
// Motor 2
const int motor2StepPin = 5;
const int motor2DirPin = 6;
const int motor2EnablePin = 7;

//Pins für die Endschalter
const int endstopXPin = 8;
const int endstopYPin = 9;

//Pins für den Elektromagneten
const int electromagnetPin = 10;
const int electromagnetPolarityPin = 11;

// AccelStepper Objekte erstellen
AccelStepper Motor1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
AccelStepper Motor2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

// MultiStepper Objekt erstellen
MultiStepper Motoren;

void processSerialInput(String input) {
  int commaIndex = input.indexOf(',');
  if (commaIndex > 0) {
    int x = input.substring(0, commaIndex).toInt();
    int y = input.substring(commaIndex + 1).toInt();
    moveToPosition(x, y, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  }
}

// LED Objekt erstellen
LED led(13, 22, 12);

void setup() {
  Serial.begin(9600);

  // Initialisierung der LED
  led.begin();

  

  // Motor 1
  pinMode(motor1StepPin, OUTPUT);
  pinMode(motor1DirPin, OUTPUT);
  pinMode(motor1EnablePin, OUTPUT);
  Motor1.setMaxSpeed(4000.0);
  Motor1.setAcceleration(2000.0);
  // Motor 2
  pinMode(motor2StepPin, OUTPUT);
  pinMode(motor2DirPin, OUTPUT);
  pinMode(motor2EnablePin, OUTPUT);
  Motor2.setMaxSpeed(4000.0);
  Motor2.setAcceleration(2000.0);
  // Füge Motor1 und Motor2 zum MultiStepper Motoren hinzu
  Motoren.addStepper(Motor1);
  Motoren.addStepper(Motor2);
  // Endschalter
  pinMode(endstopXPin, INPUT_PULLUP);
  pinMode(endstopYPin, INPUT_PULLUP);


  // Motoren aktivieren
  enableMotors(motor1EnablePin, motor2EnablePin);

  // Motoren in die Homeposition fahren
  homeMotors(Motoren, Motor1, Motor2, endstopXPin, endstopYPin, maxXPosition, maxYPosition, minXPosition, minYPosition, currentXPosition, currentYPosition);
  delay(2000);

  Move move;

  // Situation Von Garage aus
  move = {20, 350, 80, 100};
  moveStone(move, verticalLanePositions, horizontalLanePositions, computerGaragePosition, playerGaragePosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  delay(2000);

  // Situation gleiche Fahrbahn
  move = {200, 100, 200, 200};
  moveStone(move, verticalLanePositions, horizontalLanePositions, computerGaragePosition, playerGaragePosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  delay(2000);

  // Situation unterschiedliche Fahrbahnen
  move = {80, 100, 200, 200};
  moveStone(move, verticalLanePositions, horizontalLanePositions, computerGaragePosition, playerGaragePosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  delay(2000);
}

void loop() {
  /*
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    processSerialInput(input);
    Serial.println(input);
  }
  */

}