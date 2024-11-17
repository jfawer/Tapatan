// Funktionen zur Ansteuerung der Motoren

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <AccelStepper.h>

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
const int stepsPerRevolution = 600; // Schritte pro Umdrehung
const int diameter = 40; // Durchmesser des Riemenantriebs in mm

// AccelStepper Objekte erstellen
AccelStepper Motor1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
AccelStepper Motor2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

void moveToPosition(int x, int y) {
  // Berechne die Schritte für Motor 1
  int m1StepsToDo = ((x + y) * stepsPerRevolution) / (2 * PI * diameter);
  // Berechne die Schritte für Motor 2
  int m2StepsToDo = ((x - y) * stepsPerRevolution) / (2 * PI * diameter);
  // Setze die Schritte für die Motoren
  Motor1.moveTo(m1StepsToDo);
  Motor2.moveTo(m2StepsToDo);
}

void setupMotors() {
  // Motor 1
  pinMode(motor1StepPin, OUTPUT);
  pinMode(motor1DirPin, OUTPUT);
  pinMode(motor1EnablePin, OUTPUT);
  Motor1.setMaxSpeed(1000);
  Motor1.setAcceleration(1000);
  // Motor 2
  pinMode(motor2StepPin, OUTPUT);
  pinMode(motor2DirPin, OUTPUT);
  pinMode(motor2EnablePin, OUTPUT);
  Motor2.setMaxSpeed(1000);
  Motor2.setAcceleration(1000);
  // Endschalter
  pinMode(endstopXPin, INPUT_PULLUP);
  pinMode(endstopYPin, INPUT_PULLUP);
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


#endif