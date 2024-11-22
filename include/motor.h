// Funktionen zur Ansteuerung der Motoren

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <AccelStepper.h>

#include "struct.h"

// ====================================================================================================
// Funktionen zur Ermittlung der Zielposition bzw. Startposition der Motorbewegung
// ====================================================================================================

// Funktion zum Ermitteln der Startposition der Spielsteine
int findGaragestate(int garagestate[4], int value) {
  for (int i = 0; i < 4; i++) {
    if (garagestate[i] == value) {
      return i;
    }
  }
  return -1;
}

// Funktion für das Überlagern zweier Boards
void overlayBoard(int Board[3][3], int BoardMemory[3][3], int BoardOverlay[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != 0) {
        BoardOverlay[i][j] = Board[i][j];
      } else {
        BoardOverlay[i][j] = BoardMemory[i][j];
      }
    }
  }
}

// Funktion für das Ermitteln der Start und Zielposition der Motorbewegung (Platzierung der Spielsteine)
Move determineMoveToPlace(int Board[3][3], int BoardMemory[3][3], int computerGarage[4], int computerGaragePosition[5][2], int BoardPosition[3][3][2]) {
  Move move;

  // Ermitteln der Startposition / Besetzer Stein in der Garage
  int garagePosition = findGaragestate(computerGarage, 1);
  move.startRow = computerGaragePosition[garagePosition][0];
  move.startCol = computerGaragePosition[garagePosition][1];
  
  // Ermitteln der Zielposition
  BoardField targetField = getChangedField(Board, BoardMemory);
  move.targetRow = BoardPosition[targetField.row][targetField.col][0];
  move.targetCol = BoardPosition[targetField.row][targetField.col][1];

  return move;
}

// Funktion für das Ermitteln der Start und Zielposition der Motorbewegung (Verschieben der Spielsteine)
Move determineMoveToMove(int Board[3][3], int BoardMemory[3][3], int BoardPosition[3][3][2]) {
  Move move;
  int BoardOverlay[3][3];

  // Überlagern der beiden Boards
  overlayBoard(Board, BoardMemory, BoardOverlay);
  
  // Ermitteln der Startposition
  BoardField startField = getChangedField(BoardOverlay, BoardMemory);
  move.startRow = BoardPosition[startField.row][startField.col][0];
  move.startCol = BoardPosition[startField.row][startField.col][1];

  // Ermitteln der Zielposition
  BoardField targetField = getChangedField(BoardOverlay, Board);
  move.targetRow = BoardPosition[targetField.row][targetField.col][0];
  move.targetCol = BoardPosition[targetField.row][targetField.col][1];

  return move;
}

// Funktion für das Ermitteln der Start und Zielpostition der Motorbewegung (Aufräumen der Spielsteine)
Move determineCleanUpMove(int Board[3][3], int BoardPosition[3][3][2], int computerGarage[5], int computerGaragePosition[5][2], int playerGarage[5], int playerGaragePosition[5][2]) {
  Move move;
  
  // Finde die Position eines Spielsteins auf dem Spielfeld
  BoardField BoardField;
  int stoneType;
  bool found = false;

  for (int i = 0; i < 3 && !found; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != 0) {
        BoardField.row = i;
        BoardField.col = j;
        stoneType = Board[i][j];
        found = true;  // Markiere als gefunden
        break;         // Innere Schleife verlassen
      }
    }
  }

  // Ermitteln der Startposition
  move.startRow = BoardPosition[BoardField.row][BoardField.col][0];
  move.startCol = BoardPosition[BoardField.row][BoardField.col][1];

  // Ermitteln eines freien Platzes und setzen der Zielposition
  if (stoneType == Player1) {
    int garagePosition = findGaragestate(playerGarage, Leer);
    move.targetRow = playerGaragePosition[garagePosition][0];
    move.targetCol = playerGaragePosition[garagePosition][1];
  } else {
    int garagePosition = findGaragestate(computerGarage, Leer);
    move.targetRow = computerGaragePosition[garagePosition][0];
    move.targetCol = computerGaragePosition[garagePosition][1];
  }

  return move;
}

// Funktion für das Ermitteln der kürzesten Distanz zwischen einer Start und mehreren Zielpositionen
int determineShortestDistance(int col, int axisLane[2]) {
  int distance1 = abs(axisLane[0] - col);
  int distance2 = abs(axisLane[1] - col);

  if (distance1 < distance2) {
    return 0;
  } else  if (distance1 == distance2) {
    return 0;
  } else {
    return 1;
  }
}


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