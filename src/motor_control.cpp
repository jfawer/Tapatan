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

// Maximale und Minimale Positionen in mm
const int maxXPosition = 400;
const int maxYPosition = 500;
const int minXPosition = 0;
const int minYPosition = 0;

// Konstanten für die Schrittmotoren
const float stepsPerRevolution = 800.0; // Schritte pro Umdrehung
const float diameter = 10.0; // Durchmesser des Riemenantriebs in mm
int currentXPosition = 0;
int currentYPosition = 0;

// AccelStepper Objekte erstellen
AccelStepper Motor1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
AccelStepper Motor2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

// MultiStepper Objekt erstellen
MultiStepper Motoren;

void processSerialInput(String input);
void moveToPosition(int x, int y);
void enableMotors();
void disableMotors();

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
    enableMotors();
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

void processSerialInput(String input) {
  int commaIndex = input.indexOf(',');
  if (commaIndex > 0) {
    int x = input.substring(0, commaIndex).toInt();
    int y = input.substring(commaIndex + 1).toInt();
    moveToPosition(x, y);
  }
}

void moveToPosition(int x, int y) {
  // Berechne die Differenz der Positionen
  int deltaX = x - currentXPosition;
  int deltaY = y - currentYPosition;
  currentXPosition = x;
  currentYPosition = y;
  // Setze die aktuelle Position der Motoren auf 0, damit die Schritte relativ zur aktuellen Position berechnet werden
  Motor1.setCurrentPosition(0);
  Motor2.setCurrentPosition(0);
  // Berechne die Schritte für Motor 1
  float m1StepsToDo = ((deltaX + deltaY) * stepsPerRevolution) / (PI * diameter);
  m1StepsToDo = round(m1StepsToDo);
  // Berechne die Schritte für Motor 2
  float m2StepsToDo = ((deltaX - deltaY) * stepsPerRevolution) / (PI * diameter);
  m2StepsToDo = round(m2StepsToDo);

  // Setze die Schritte für die Motoren
  long positions[2] = {(int)m1StepsToDo, (int)m2StepsToDo};
  Motoren.moveTo(positions);
}

void enableMotors() {
  // Motor 1
  digitalWrite(motor1EnablePin, LOW);
  // Motor 2
  digitalWrite(motor2EnablePin, LOW);
}

void disableMotors() {
  // Motor 1
  digitalWrite(motor1EnablePin, HIGH);
  // Motor 2
  digitalWrite(motor2EnablePin, HIGH);
}