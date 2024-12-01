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
#include "led.h"
#include "motor.h"

// Maximale und Minimale Positionen in mm
const int maxXPosition = 400;
const int maxYPosition = 500;
const int minXPosition = 0;
const int minYPosition = 0;

int playerGaragePosition[5][2] = {                                                                                  // Positionen der Garagen des Spielers
  {34, 25}, 
  {34, 20}, 
  {34, 15}, 
  {34, 10},
  {34, 5}
};

int computerGaragePosition[5][2] = {                                                                                // Positionen der Garagen des Computers
  {6, 25}, 
  {6, 20},
  {6, 15}, 
  {6, 10}, 
  {6, 5}
};                                                                                                                  

int BoardPosition[3][3][2] = {                                                                                      // Positionen der Spielfelder
  {{14, 20}, {20, 20}, {26, 20}}, 
  {{14, 15}, {20, 15}, {26, 15}}, 
  {{14, 10}, {20, 10}, {26, 10}}
};        

int verticalLanePositions[2] = {12, 17};                                                                            // Array für die vertikalen Fahrbahnen
int horizontalLanePositions[2] = {9, 30};                                                                           // Array für die horizontalen Fahrbahnen

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
    moveToPosition(x, y, Motoren, Motor1, Motor2, currentXPosition, currentYPosition);
  }
}

void setup() {
  Serial.begin(9600);
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
  enableMotors(motor1EnablePin, motor2EnablePin);
}

void loop() {
 if (Serial.available() > 0) {
  String input = Serial.readStringUntil('\n');
  processSerialInput(input);
  Serial.println(input);
 }
 Motoren.run();
 //Motoren.runSpeedToPosition();     //stellt sicher, das die Motoren zu ihrer Zielposition mit der vorgeschriebenen Beschleunigung und Geschwindigkeit fahren. Blockirt aber!!!
}