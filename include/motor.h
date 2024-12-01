// Funktionen zur Ansteuerung der Motoren

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "struct.h"
#include "game_logic.h"

// ====================================================================================================
// Funktionen zur Ermittlung der Zielposition bzw. Startposition der Motorbewegung
// ====================================================================================================

// Funktion für das Ermitteln eines Garagenzustands (belegt oder leer)
int findGaragestate(int garagestate[4], int value) {  
  for (int i = 0; i < 4; i++) {
    if (garagestate[i] == value) {
      return i;
    }
  }
  return -1;
}

// Funktion für das Überlagern zweier Spielfelder (Board und BoardMemory) und speichern des Ergebnisses in einem dritten Spielfeld (BoardOverlay)
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

// Funktion zum Erkennen des geänderten Feldes im Spielfeld
BoardField identifyChangedField(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return {i, j}; // Rückgabe des geänderten Feldes
      }
    }
  }
  return {-1, -1}; // Rückgabe von (-1, -1), wenn kein Feld geändert wurde
}

// Funktion für das Ermitteln der Start und Zielposition der Motorbewegung (Platzierung der Spielsteine)
Move determineMoveToPlace(int Board[3][3], int BoardMemory[3][3], int computerGarage[4], int computerGaragePosition[5][2], int BoardPosition[3][3][2]) {
  Move move;

  // Ermitteln der Startposition / Besetzer Stein in der Garage
  int garagePosition = findGaragestate(computerGarage, 1);
  move.startRow = computerGaragePosition[garagePosition][0];
  move.startCol = computerGaragePosition[garagePosition][1];
  
  // Ermitteln der Zielposition
  BoardField targetField = identifyChangedField(Board, BoardMemory);
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
  BoardField startField = identifyChangedField(BoardOverlay, BoardMemory);
  move.startRow = BoardPosition[startField.row][startField.col][0];
  move.startCol = BoardPosition[startField.row][startField.col][1];

  // Ermitteln der Zielposition
  BoardField targetField = identifyChangedField(BoardOverlay, Board);
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


// ====================================================================================================
// Testfunktionen für die Motorbewegung
// ====================================================================================================

// Funktion für die Ausgabe der Motorbewegung
void SerialMoveToPostion(int x, int y) {
  Serial.println("Move to Position: " + String(x) + ", " + String(y));
}


// ====================================================================================================
// Funktionen (Logik) für die Motorbewegung
// ====================================================================================================

// Funktion für das Ermitteln der kürzesten Fahrbahn
int determineShortestLane(int col, int axisLane[2]) {
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

// Funktion zum bestimmen, ob Start und Zielposition beide die gleiche kürzeste Fahrbahn haben
bool isSameLane(int startCol, int targetCol, int axisLane[2]) {
  int startLane = determineShortestLane(startCol, axisLane);
  int targetLane = determineShortestLane(targetCol, axisLane);

  return startLane == targetLane;
}

// Funktion zum Überprüfen, ob sich die Position in einer Garage befindet
bool isInGarage(int row, int col, int garagePosition[5][2]) {
  for (int i = 0; i < 5; i++) {
    if (row == garagePosition[i][0] && col == garagePosition[i][1]) {
      return true;
    }
  }
  return false;
}

// Funktion für die Bewegung des Motors je nach Spielzug (Platzierung, Verschieben, Aufräumen)
void moveStone(Move move, int verticalLanePositions[2], int horizontalLanePositions[2], int computerGaragePosition[5][2]) {

  // Überprüfen, ob sich die Startposition in einer Garage befindet
  if (isInGarage(move.startRow, move.startCol, computerGaragePosition)) {

    // Bestimmen der Fahrbahnen für die Bwegung
    int lane = determineShortestLane(move.targetCol, verticalLanePositions);
    int horizontalLane = determineShortestLane(move.startRow, horizontalLanePositions);

    SerialMoveToPostion(move.startRow, move.startCol);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], move.startCol);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, move.targetCol);
    return;
  }

  // Überprüfen, ob sich die Zielposition in einer Garage befindet
  if (isInGarage(move.targetCol, move.targetCol, computerGaragePosition)) {
    // Bestimmen der Fahrbahnen für die Bewegung
    int lane = determineShortestLane(move.startCol, verticalLanePositions);
    int horizontalLane = determineShortestLane(move.targetRow, horizontalLanePositions);

    SerialMoveToPostion(move.startRow, move.startCol);
    delay(200);
    SerialMoveToPostion(move.startRow, verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], move.targetCol);
    delay(200);
    SerialMoveToPostion(move.targetRow, move.targetCol);
    return;
  }
  
  // Überprüfen, ob sich die Start- und Zielposition auf der gleichen Fahrbahn befinden
  if (isSameLane(move.startCol, move.targetCol, verticalLanePositions)) {
    // Bestimmen der gemeinsamen Fahrbahn
    int lane = determineShortestLane(move.startCol, verticalLanePositions);

    SerialMoveToPostion(move.startRow, move.startCol);
    delay(200);
    SerialMoveToPostion(move.startRow, verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, verticalLanePositions[lane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, move.targetCol);
    return;

  } else {
    // Bestimmen der kürzesten Fahrbahn für Start- und Zielposition
    int startLane = determineShortestLane(move.startCol, verticalLanePositions);
    int targetLane = determineShortestLane(move.targetCol, verticalLanePositions);
    int horizontalLane = determineShortestLane(move.targetRow, horizontalLanePositions);

    SerialMoveToPostion(move.startRow, move.startCol);
    delay(200);
    SerialMoveToPostion(move.startRow, verticalLanePositions[startLane]);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], verticalLanePositions[startLane]);
    delay(200);
    SerialMoveToPostion(horizontalLanePositions[horizontalLane], verticalLanePositions[targetLane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, verticalLanePositions[targetLane]);
    delay(200);
    SerialMoveToPostion(move.targetRow, move.targetCol);
    return;
  }
}

// ====================================================================================================
// Funktionen für die Motorsteuerung
// ====================================================================================================

// Funktion zur Berechnung der Schritte basierend auf Delta und Motorparametern
float calculateSteps(float delta) {
  // Konstanten der Schrittmotoren
  const float stepsPerRevolution = 800.0;   // Schritte pro Umdrehung
  const float diameter = 10.0;              // Durchmesser des Riemenantriebs in mm
  
  return round((delta * stepsPerRevolution) / (PI * diameter));
}

// Funktion für die Bewegung zu einer bestimmten Position
void moveToPosition(int x, int y, MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int& currentXPosition, int& currentYPosition) {
  // Berechne die Differenz der Positionen
  int deltaX = x - currentXPosition;
  int deltaY = y - currentYPosition;
  currentXPosition = x;
  currentYPosition = y;

  // Setze die aktuelle Position der Motoren auf 0, damit die Schritte relativ zur aktuellen Position berechnet werden
  Motor1.setCurrentPosition(0);
  Motor2.setCurrentPosition(0);
  
  // Berechne die Schritte für Motor 1 und Motor 2 basierend auf der Differenz der Positionen
  float m1StepsToDo = calculateSteps(deltaX + deltaY);
  float m2StepsToDo = calculateSteps(deltaX - deltaY);

  // Setze die Schritte für die Motoren
  long positions[2] = {(int)m1StepsToDo, (int)m2StepsToDo};
  Motoren.moveTo(positions);
}

void enableMotors(int motor1EnablePin, int motor2EnablePin) {
  digitalWrite(motor1EnablePin, LOW);     // Motor 1
  digitalWrite(motor2EnablePin, LOW);     // Motor 2
}

void disableMotors(int motor1EnablePin, int motor2EnablePin) {
  digitalWrite(motor1EnablePin, HIGH);    // Motor 1
  digitalWrite(motor2EnablePin, HIGH);    // Motor 2
}

void homeMotors(MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int endstopXPin, int endstopYPin, int maxXPosition, int maxYPosition, int minXPosition, int minYPosition,  int& currentXPosition, int& currentYPosition) {
  // Aktuelle Position auf die weit entferteste Position setzen
  currentXPosition = maxXPosition;
  currentYPosition = maxYPosition;
  // Y-Achse entlang fahren, bis der Endschalter erreicht wird
  moveToPosition(maxXPosition, minYPosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition);
  while (digitalRead(endstopYPin) == HIGH) {
    Motoren.run();
  }
  // X-Achse entlang fahren, bis der Endschalter erreicht wird
  moveToPosition(minXPosition, minYPosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition);
  while (digitalRead(endstopXPin) == HIGH) {
    Motoren.run();
  }
  // Aktuelle Position auf 0 setzen
  currentXPosition = 0;
  currentYPosition = 0;
}

// ====================================================================================================
// Funktionen für die Elektromagnetsteuerung
// ====================================================================================================

//Funktion zur Ansteuerung des Elektromagneten
void electromagnetControl(int electromagnetPin, int electromagnetPolarityPin, bool state, bool polarity) {
  digitalWrite(electromagnetPin, LOW);
  digitalWrite(electromagnetPolarityPin, polarity);
  digitalWrite(electromagnetPin, state);
}

#endif