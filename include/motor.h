// Funktionen zur Ansteuerung der Motoren

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

/*
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
  int m1StepsToDo = ((deltaX + deltaY) * stepsPerRevolution) / (2 * PI * diameter);
  // Berechne die Schritte für Motor 2
  int m2StepsToDo = ((deltaX - deltaY) * stepsPerRevolution) / (2 * PI * diameter);
  // Setze die Schritte für die Motoren
  long positions[2] = {m1StepsToDo, m2StepsToDo};
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
*/


#endif