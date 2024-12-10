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
  move.startX = computerGaragePosition[garagePosition][0];
  move.startY = computerGaragePosition[garagePosition][1];
  
  // Ermitteln der Zielposition
  BoardField targetField = identifyChangedField(Board, BoardMemory);
  move.targetX = BoardPosition[targetField.row][targetField.col][0];
  move.targetY = BoardPosition[targetField.row][targetField.col][1];

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
  move.startX = BoardPosition[startField.row][startField.col][0];
  move.startY = BoardPosition[startField.row][startField.col][1];

  // Ermitteln der Zielposition
  BoardField targetField = identifyChangedField(BoardOverlay, Board);
  move.targetX = BoardPosition[targetField.row][targetField.col][0];
  move.targetY = BoardPosition[targetField.row][targetField.col][1];

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
  move.startX = BoardPosition[BoardField.row][BoardField.col][0];
  move.startY = BoardPosition[BoardField.row][BoardField.col][1];

  // Ermitteln eines freien Platzes und setzen der Zielposition
  if (stoneType == Player1) {
    int garagePosition = findGaragestate(playerGarage, Leer);
    move.targetX = playerGaragePosition[garagePosition][0];
    move.targetY = playerGaragePosition[garagePosition][1];
  } else {
    int garagePosition = findGaragestate(computerGarage, Leer);
    move.targetX = computerGaragePosition[garagePosition][0];
    move.targetY = computerGaragePosition[garagePosition][1];
  }

  return move;
}


/*
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

// Funktion zum Setzen der Zielposition der Motoren
void setPosition(int x, int y, MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int& currentXPosition, int& currentYPosition, int maxXPosition, int maxYPosition, int minXPosition, int minYPosition) {
  if (x <= maxXPosition && x >= minXPosition && y <= maxYPosition && y >= minYPosition) {
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
  } else {
    // Fehlermeldung, wenn die Position außerhalb des erlaubten Bereichs liegt
    Serial.println("Position ausserhalb des erlaubten Bereichs");
  }
}

// Funktionen zur Bewegung zu einer vorbereiteten Position
void moveToPosition(int x, int y, MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int& currentXPosition, int& currentYPosition, int maxXPosition, int maxYPosition, int minXPosition, int minYPosition) {
  // Setze die Zielposition
  setPosition(x, y, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);

  // Bewegung ausführen, bis die Zielposition erreicht ist
  Motoren.runSpeedToPosition(); // Blockiert, bis alle Motoren an ihrer Zielposition angekommen sind
}

// Funktion zur Freigabe der Motoren
void enableMotors(int motor1EnablePin, int motor2EnablePin) {
  digitalWrite(motor1EnablePin, LOW);     // Motor 1
  digitalWrite(motor2EnablePin, LOW);     // Motor 2
}

// Funktion zum Deaktivieren der Motoren
void disableMotors(int motor1EnablePin, int motor2EnablePin) {
  digitalWrite(motor1EnablePin, HIGH);    // Motor 1
  digitalWrite(motor2EnablePin, HIGH);    // Motor 2
}

// Funktion zum 0-Positionieren der Motoren
void homeMotors(MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int endstopXPin, int endstopYPin, int maxXPosition, int maxYPosition, int minXPosition, int minYPosition, int& currentXPosition, int& currentYPosition) {
  // Aktuelle Position auf die weit entfernte Position setzen
  currentXPosition = maxXPosition;
  currentYPosition = maxYPosition;

  // Y-Achse entlang fahren, bis der Endschalter erreicht wird
  setPosition(maxXPosition, minYPosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  while (digitalRead(endstopYPin) != LOW) {
    Motoren.run(); // Bewegung manuell steuern, bis der Endschalter ausgelöst wird
  }

  // X-Achse entlang fahren, bis der Endschalter erreicht wird
  setPosition(minXPosition, minYPosition, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
  while (digitalRead(endstopXPin) != LOW) {
    Motoren.run(); // Bewegung manuell steuern, bis der Endschalter ausgelöst wird
  }

  // AKtuelle Position auf 0 setzen
  currentXPosition = minXPosition;
  currentYPosition = minYPosition;

  // Setze die aktuelle Position der Motoren auf (0,0)
  Motor1.setCurrentPosition(0);
  Motor2.setCurrentPosition(0);
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
    return 1;
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
bool isInGarage(int row, int col, int computerGaragePosition[5][2], int playerGaragePosition[5][2]) {
  for (int i = 0; i < 5; i++) {
    if (computerGaragePosition[i][0] == row && computerGaragePosition[i][1] == col) {
      return true;
    }
    if (playerGaragePosition[i][0] == row && playerGaragePosition[i][1] == col) {
      return true;
    }
  }
  return false;
}

// Funktion für die Bewegung des Motors je nach Spielzug (Platzierung, Verschieben, Aufräumen)
void moveStone(Move move, int verticalLanePositions[2], int horizontalLanePositions[2], int computerGaragePosition[5][2], int playerGaragePosition[5][2], MultiStepper& Motoren, AccelStepper& Motor1, AccelStepper& Motor2, int& currentXPosition, int& currentYPosition, int maxXPosition, int maxYPosition, int minXPosition, int minYPosition) {
  int startdelay = 2000;
  int delayTime = 500;

  // Überprüfen, ob sich die Startposition in einer Garage befindet
  if (isInGarage(move.startX, move.startY, computerGaragePosition, playerGaragePosition)) {
    // Bestimmen der Fahrbahnen für die Bwegung
    int lane = determineShortestLane(move.startY, verticalLanePositions);               // Vertikale Fahrbahn
    int horizontalLane = determineShortestLane(move.targetX, horizontalLanePositions);  // Horizontale Fahrbahn
    
    // Bewegung des Motors
    moveToPosition(move.startX, move.startY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(startdelay);
    moveToPosition(move.startX, verticalLanePositions[lane], Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[horizontalLane], verticalLanePositions[lane], Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[horizontalLane], move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(move.targetX, move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    return;
  }

  // Überprüfen, ob sich die Start- und Zielposition auf der gleichen Fahrbahn befinden
  if (isSameLane(move.startX, move.targetX, horizontalLanePositions)) {
    // Bestimmen der gemeinsamen Fahrbahn
    int lane = determineShortestLane(move.startX, horizontalLanePositions);              // Gemeinsame Fahrbahn

    // Bewegung des Motors
    moveToPosition(move.startX, move.startY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(startdelay);
    moveToPosition(horizontalLanePositions[lane], move.startY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[lane], move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(move.targetX, move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    return;

  } else {
    // Bestimmen der kürzesten Fahrbahn für Start- und Zielposition
    int startLane = determineShortestLane(move.startX, horizontalLanePositions);          // Startfahrbahn (Horizontale Fahrbahn)
    int targetLane = determineShortestLane(move.targetX, horizontalLanePositions);        // Zielfahrbahn  (Horizontale Fahrbahn)
    int verticalLane = determineShortestLane(move.targetY, verticalLanePositions);        // Vertikale Fahrbahn

    // Bewegung des Motors
    moveToPosition(move.startX, move.startY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(startdelay);
    moveToPosition(horizontalLanePositions[startLane], move.startY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[startLane], verticalLanePositions[verticalLane], Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[targetLane], verticalLanePositions[verticalLane], Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(horizontalLanePositions[targetLane], move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    delay(delayTime);
    moveToPosition(move.targetX, move.targetY, Motoren, Motor1, Motor2, currentXPosition, currentYPosition, maxXPosition, maxYPosition, minXPosition, minYPosition);
    return;
  }
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

*/

#endif