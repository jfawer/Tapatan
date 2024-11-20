// Funktionen zur Ansteuerung der Motoren

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

void moveToPosition(int x, int y) {
  // Berechne die Schritte für Motor 1
  int m1StepsToDo = ((x + y) * stepsPerRevolution) / (2 * PI * diameter);
  // Berechne die Schritte für Motor 2
  int m2StepsToDo = ((x - y) * stepsPerRevolution) / (2 * PI * diameter);
  // Setze die Schritte für die Motoren
  Motor1.moveTo(m1StepsToDo);
  Motor2.moveTo(m2StepsToDo);
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