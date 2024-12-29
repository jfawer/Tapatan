// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Einbinden der Bibliotheken
#include <Arduino.h>

// Einbinden der Header-Dateien
#include <limits.h>
#include "struct.h"
#include "input.h"
#include "display.h"

// ====================================================================================================
// Hilfsfunktionen für die Spiellogik
// ====================================================================================================

// Funktion zum Kopieren eines Arrays in ein anderes Array
void copyBoard(int sourceBoard[3][3], int destinationBoard[3][3]) {  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      destinationBoard[i][j] = sourceBoard[i][j];
    }
  }
}

// Funktion zum Wechseln des aktuellen Spielers (Von 1 zu 2 und umgekehrt)
void switchPlayer(int &currentPlayer) {
  currentPlayer = (currentPlayer == 1) ? 2 : 1; 
}

// Funktion zum Erkennen des geänderten Feldes im Spielfeld
BoardField getChangedField(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return {i, j}; // Rückgabe des geänderten Feldes
      }
    }
  }
  return {-1, -1}; // Rückgabe von (-1, -1), wenn kein Feld geändert wurde
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

// ====================================================================================================
// Tic-Tac-Toe: Funktionen für die Spiellogik des Spielers
// ====================================================================================================

// Funktion zur Erkennung, ob sich das Spielfeld geändert hat
bool hasBoardChanged(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zum Überprüfen, ob der Zug gültig ist
bool isValidMove(int currentBoard[3][3], int savedBoard[3][3], int currentPlayer) {
    BoardField changedField = getChangedField(currentBoard, savedBoard);
    int row = changedField.row;
    int col = changedField.col;

    if (row == -1 && col == -1) {
        return false; // Kein Feld wurde geändert
    }

    int oldValue = savedBoard[row][col];   
    int newValue = currentBoard[row][col];

    if (oldValue == 0 && newValue == currentPlayer) {   // Gültiger Zug, wenn das Feld leer war und der Spieler das Feld geändert hat
        return true;
    } else {
        return false;
    }
}

// Funktion zum Überprüfen ob das aktuelle Spielfeld dem gespeicherten Spielfeld entspricht
bool isBoardEqual(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return false;
      }
    }
  }
  return true;
}

// Funktion zum Warten, bis das Spielfeld wieder dem gespeicherten Spielfeld entspricht
void awaitBoardIsEqual(int currentBoard[3][3], int savedBoard[3][3], const int potPins[]) {
  while (!isBoardEqual(currentBoard, savedBoard)) {
    updateBoard(currentBoard, potPins);
    Serial.println("Board: ");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(currentBoard[i][j]);
        Serial.print(" ");
      }
      Serial.println();
    }
    delay(100);
  }
}

// Funktion zur Handhabung eines illegalen Zugs
void handleIllegalMove(LiquidCrystal_I2C &lcd, int Board[3][3], int BoardMemory[3][3], const int potPins[], GameSettings gameSettings, int currentPlayer) {
  displayBoard(lcd, BoardMemory);                                                     // Zeigt den alten Zustand des Spielfelds
  displayIllegalMove(lcd);                                                            // Zeigt eine Meldung für einen illegalen Zug
  awaitBoardIsEqual(Board, BoardMemory, potPins);                                     // Wartet, bis das Spielfeld zurückgesetzt ist
  displayPlayer(lcd, gameSettings, currentPlayer);                                    // Zeigt den Spieler, der am Zug ist, erneut an
}

// Funktion für das Platzieren eines Spielsteins durch den Spieler
bool playerPlaces(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool turnOver = false;
    
  if (hasBoardChanged(Board, BoardMemory)) {
    if (isValidMove(Board, BoardMemory, currentPlayer)) {
      displayBoard(lcd, Board);
      turnOver = true;
    } else {
      handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);
    }
  }
  return turnOver;
}


// ====================================================================================================
// Tic-Tac-Toe: Funktionen für die Spiellogik des Computers / Spielfeldauswertung
// ====================================================================================================

// Funktion zur Überprüfung, ob noch Felder frei sind
bool checkFieldsLeft(int Board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == 0) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zur Bewertung des Spielfelds
int evaluateBoard(int Board[3][3]) {
    // Überprüfen, ob eine Reihe oder Spalte gewonnen hat
    for (int i = 0; i < 3; i++) {
        if (Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2] && Board[i][0] != 0) {
            return (Board[i][0] == 1) ? 10 : (Board[i][0] == 2) ? -10 : 0;
        }
        if (Board[0][i] == Board[1][i] && Board[1][i] == Board[2][i] && Board[0][i] != 0) {
            return (Board[0][i] == 1) ? 10 : (Board[0][i] == 2) ? -10 : 0;
        }
    }

    // Überprüfen, ob eine Diagonale gewonnen hat
    if (Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2] && Board[0][0] != 0) {
        return (Board[0][0] == 1) ? 10 : (Board[0][0] == 2) ? -10 : 0;
    }
    if (Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0] && Board[0][2] != 0) {
        return (Board[0][2] == 1) ? 10 : (Board[0][2] == 2) ? -10 : 0;
    }
    return 0;
}

// Funktion zur Überprüfung, ob das Spiel vorbei ist
bool isGameOver(int Board[3][3]) {
  // Überprüfen, ob jemand gewonnen hat
  if (evaluateBoard(Board) != 0) {
    return true;
  }
  // Überprüfen, ob noch Felder frei sind
  return !checkFieldsLeft(Board);
}

// Funktion zur Generierung aller möglichen nächsten Zustände
void getChildren(int Board[3][3], int children[9][3][3], BoardField moves[9], int &numChildren, bool maximizingPlayer) {
  numChildren = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == 0) {
        // Kopiere das aktuelle Board
        copyBoard(Board, children[numChildren]);
        // Setze den Zug
        children[numChildren][i][j] = maximizingPlayer ? 1 : 2;
        // Speichere den Zug
        moves[numChildren].row = i;
        moves[numChildren].col = j;
        numChildren++;
      }
    }
  }
}

// Minimax-Algorithmus mit Alpha-Beta-Pruning
int minimax(int Board[3][3], int depth, int alpha, int beta, bool maximizingPlayer, BoardField &bestMove) {
  if (depth == 0 || isGameOver(Board)) {
    return evaluateBoard(Board);
  }

  if (maximizingPlayer) {
    int maxEval = INT_MIN;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;
    getChildren(Board, children, moves, numChildren, true);
    for (int i = 0; i < numChildren; i++) {
      int eval = minimax(children[i], depth - 1, alpha, beta, false, currentMove);
      if (eval > maxEval) {
        maxEval = eval;
        bestMove = moves[i]; // Speichere die tatsächliche Position
      }
      alpha = max(alpha, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;
    getChildren(Board, children, moves, numChildren, false);
    for (int i = 0; i < numChildren; i++) {
      int eval = minimax(children[i], depth - 1, alpha, beta, true, currentMove);
      if (eval < minEval) {
        minEval = eval;
        bestMove = moves[i]; // Speichere die tatsächliche Position
      }
      beta = min(beta, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return minEval;
  }
}

// Funktion zur Bestimmung eines zufälligen Zuges für den Computer und Einfügen in das Board
void makeRandomMove(int Board[3][3]) {
  BoardField randomMove;
  do {
    randomMove.row = random(0, 3);
    randomMove.col = random(0, 3);
  } while (Board[randomMove.row][randomMove.col] != 0);

  Board[randomMove.row][randomMove.col] = 2;                          // Setze den Zug für Spieler 2
}

// Funktion zur Bestimmung des besten Zuges für den Computer und Einfügen in das Board
void makeBestMove(int Board[3][3]) {
  BoardField bestMove;                                                // Speichert den besten Zug
  minimax(Board, 9, INT_MIN, INT_MAX, false, bestMove);               // Computer (minimizingPlayer) am Zug
  Board[bestMove.row][bestMove.col] = 2;                              // Setze den Zug für den Computer
}


// ====================================================================================================
// Tapatan: Funktionen für die Spiellogik des Spielers
// ====================================================================================================

// Funktion zum Zählen der Spielsteine auf dem Spielfeld eines Spielers
int countPlayerStones(int Board[3][3], int player) {
  int count = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (Board[i][j] == player) {
            count++;
        }
    }
  }
  return count;
}

// Funktion zum Überprüfen, ob ein gültiger Spielstein zum Verschieben ausgewählt wurde
bool isValidPick(int currentBoard[3][3], BoardField originField) {
  int row = originField.row;
  int col = originField.col;

  // Durchlaufen der benachbarten Felder im Umkreis von 1
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      // Benachbartes Feld berechnen
      int neighborRow = row + i;
      int neighborCol = col + j;

      // Überprüfen, ob die Position innerhalb des Spielfelds liegt
      if (neighborRow >= 0 && neighborRow < 3 && neighborCol >= 0 && neighborCol < 3) {
        // Überspringen des Ursprungsfeldes selbst
        if (neighborRow == row && neighborCol == col) {
          continue;
        }
        // Überprüfen, ob das benachbarte Feld leer ist
        if (currentBoard[neighborRow][neighborCol] == 0) {
          return true; // Ein leeres benachbartes Feld wurde gefunden
        }
      }
    }
  }
  return false; // Kein leeres benachbartes Feld gefunden
}

// Funktion zum Überprüfen, ob ein gültiger Zug durchgeführt wurde
bool isValidPlacement(int currentBoard[3][3], int savedBoard[3][3], int currentPlayer, BoardField originField) {
  BoardField changedField = getChangedField(currentBoard, savedBoard);
  int row = changedField.row;
  int col = changedField.col;

  if (row == -1 && col == -1) {
    return false;// Kein Feld wurde geändert
  }

  int oldValue = savedBoard[row][col];   
  int newValue = currentBoard[row][col];

  // Überprüfen, ob das geänderte Feld leer war, vom aktuellen Spieler geändert wurde,
  // nicht das Ursprungsfeld ist und innerhalb eines Radius von 1 Feld liegt
  if (oldValue == 0 && newValue == currentPlayer && 
    (row != originField.row || col != originField.col) &&
    abs(row - originField.row) <= 1 && abs(col - originField.col) <= 1) {
    return true;
  } else {
    return false;
  }
}

// Funktion zur Handhabung der Bewegung des Spielers
bool handlePlayerMovement(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  displayBoard(lcd, Board);                                                                   // Zeigt den aktuellen Zustand des Spielfelds
  BoardField originField = getChangedField(Board, BoardMemory);                               // Ausgewählter Spielstein

  if (!isValidPick(Board, originField)) {                                                     // Überprüfung, ob ein gültiger Spielstein ausgewählt wurde
    handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);         // Handhabung eines ungültigen Zugs
  } else {
    copyBoard(Board, BoardMemory);                                                            // Kopiert das aktuelle Spielfeld in das Speicherfeld
    bool validMove = false;
    while (!validMove) {                                                                      // Schleife, bis eine gültige Bewegung durchgeführt wurde
      updateBoard(Board, potPins); 
      if (hasBoardChanged(Board, BoardMemory)) {                                              // Überprüfen, ob sich das Spielfeld geändert hat
        if (isValidPlacement(Board, BoardMemory, currentPlayer, originField)) {               // Überprüfen, ob eine gültige Bewegung durchgeführt wurde
          displayBoard(lcd, Board);                                                           
          validMove = true;                                                                   // Beeendet die Schleife
          return true;                                                                        // Gültige Bewegung durchgeführt
        } else {
          handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);   // Handhabung eines ungültigen Zugs
        }
      }
    }
  }
  return false;                                                                               // Ungültige Bewegung durchgeführt                     
}


// ====================================================================================================
// Funktionen für die Tapatan-Spiellogik des Computers
// ====================================================================================================

// Funktion zur Generierung aller möglichen nächsten Zustände durch Verschieben der Spielsteine
void TapatangetChildren(int Board[3][3], int children[9][3][3], BoardField moves[9], int &numChildren, int currentPlayer) {
    numChildren = 0;
    
    // Durchlaufe alle Felder des Boards
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Wenn das Feld dem aktuellen Spieler gehört
            if (Board[i][j] == currentPlayer) {
                
                // Prüfe die möglichen Bewegungen (oben, unten, links, rechts, und diagonal)
                int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

                // Durchlaufe alle Richtungen
                for (int d = 0; d < 8; d++) {
                    int newRow = i + directions[d][0];
                    int newCol = j + directions[d][1];
                    
                    // Stelle sicher, dass der neue Standort innerhalb des Spielfelds liegt
                    if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3 && Board[newRow][newCol] == 0) {
                        
                        // Kopiere das aktuelle Board und verschiebe den Spielstein
                        copyBoard(Board, children[numChildren]);
                        children[numChildren][i][j] = 0;                        // Leere das Ursprungsfeld
                        children[numChildren][newRow][newCol] = currentPlayer;  // Setze den Spielstein auf das neue Feld
                        
                        // Speichere den Zug (ursprüngliche und neue Position)
                        moves[numChildren].row = newRow;
                        moves[numChildren].col = newCol;
                        numChildren++;
                    }
                }
            }
        }
    }
}

// Minimax-Algorithmus mit Alpha-Beta-Pruning für das Tapatan-Spiel
int Tapatanminimax(int Board[3][3], int depth, int alpha, int beta, bool maximizingPlayer, BoardField &bestMove, int bestChildren[3][3], int originalDepth = -1) {
  // Nur beim ersten Aufruf initialisieren
  if (originalDepth == -1) {
    originalDepth = depth;
  }

  if (depth == 0 || isGameOver(Board)) {
    int score = evaluateBoard(Board);
    return score;
  }

  if (maximizingPlayer) {
    int maxEval = INT_MIN;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;

    // Generiere mögliche Zustände für den aktuellen Spieler
    TapatangetChildren(Board, children, moves, numChildren, 1); // 1 = maximizingPlayer

    for (int i = 0; i < numChildren; i++) {
      int eval = Tapatanminimax(children[i], depth - 1, alpha, beta, false, currentMove, bestChildren, originalDepth);
      if (eval > maxEval) {
        maxEval = eval;
        bestMove = moves[i]; // Speichere den besten Zug
        // Speichere das children-Array des besten Moves nur bei der obersten Rekursionsebene
        if (depth == originalDepth) {
          memcpy(bestChildren, children[i], sizeof(children[i]));
        }
      }
      alpha = max(alpha, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;

    // Generiere mögliche Zustände für den Gegner
    TapatangetChildren(Board, children, moves, numChildren, 2); // 2 = minimizingPlayer

    for (int i = 0; i < numChildren; i++) {
      int eval = Tapatanminimax(children[i], depth - 1, alpha, beta, true, currentMove, bestChildren, originalDepth);
      if (eval < minEval) {
        minEval = eval;
        bestMove = moves[i]; // Speichere den besten Zug
        // Speichere das children-Array des besten Moves nur bei der obersten Rekursionsebene
        if (depth == originalDepth) {
          memcpy(bestChildren, children[i], sizeof(children[i]));
        }
      }
      beta = min(beta, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return minEval;
  }
}

// Funktion zur Bestimmung eines zufälligen Zuges für den Computer und Einfügen in das Board
void TapatanMakeRandomMove(int Board[3][3]) {
  int possibleMoves[8][2];                                                                            // Array für mögliche Züge
  int possibleMoveCount = 0;                                                                          // Zähler für die Anzahl der möglichen Züge
  int row, col;                                                                                       // Variablen für die Zeile und Spalte des Spielsteins
  bool validMove = false;                                                                             // Flag für die Gültigkeit des Zugs

  do {
    // Zufällige Auswahl eines Spielsteins
    row = random(0, 3);
    col = random(0, 3);
    
    if (isValidPick(Board, {row, col}) && Board[row][col] == 2) {                                     // Überprüfen, ob das Feld ein gültiges Feld ist und dem Computer gehört
      int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};  // Array für die 8 Richtungen
      
      for (int d = 0; d < 8; d++) {                                                                   // Durchlaufe alle Richtungen
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3 && Board[newRow][newCol] == 0) {   // Überprüfen, ob das Feld leer ist und innerhalb des Spielfelds liegt
          possibleMoves[possibleMoveCount][0] = newRow;                                               
          possibleMoves[possibleMoveCount][1] = newCol;                                               
          possibleMoveCount++;                                                                        // Erhöhe die Anzahl der möglichen Züge
        }
      }
      
      if (possibleMoveCount > 0) {                                                                    // Überprüfen, ob mindestens ein möglicher Zug gefunden wurde
        validMove = true;
      }
    }
  } while (!validMove);                                                                               // Wiederhole, bis ein gültiger Zug gefunden wurde

  int randomMove = random(0, possibleMoveCount);                                                      // Zufällige Auswahl unter den möglichen Zügen
  int newRow = possibleMoves[randomMove][0];
  int newCol = possibleMoves[randomMove][1];
  
  Board[row][col] = 0;                                                                                // Ursprungsfeld auf 0 setzen / leeren
  Board[newRow][newCol] = 2;                                                                          // Neues Feld auf 2 setzen
}

// Funktion zur Bestimmung des besten Zuges für den Computer und Einfügen in das Board
void TapatanMakeBestMove(int Board[3][3]) {
  BoardField bestMove;
  int bestChildBoard[3][3];
  Tapatanminimax(Board, 9, INT_MIN, INT_MAX, false, bestMove, bestChildBoard);
  copyBoard(bestChildBoard, Board);
}


// ====================================================================================================
// Funktionen für das Ermitteln der Bewegung des Motors
// ====================================================================================================

// Funktion für das Ermitteln eines Garagenzustands (belegt oder leer)
int findGaragestate(int garagestate[5], int value) {  
  for (int i = 0; i <= 4; i++) {
    if (garagestate[i] == value) {
      return i;
    }
  }
  return -1;
}

// Funktion für das Ermitteln der Start und Zielposition der Motorbewegung (Platzierung der Spielsteine)
Move determineMoveToPlace(int Board[3][3], int BoardMemory[3][3], int garageState[2][5], MotorConfig config) {
  Move move;

  // Ermitteln der Startposition / Besetzer Stein in der Garage
  int garagePosition = findGaragestate(garageState[0], 1);
  move.startX = config.computerGaragePosition[garagePosition][0];
  move.startY = config.computerGaragePosition[garagePosition][1];

  // Leeren der Garage
  garageState[0][garagePosition] = 0;
  
  // Ermitteln der Zielposition
  BoardField targetField = getChangedField(Board, BoardMemory);
  move.targetX = config.boardPosition[targetField.row][targetField.col][0];
  move.targetY = config.boardPosition[targetField.row][targetField.col][1];
  return move;
}

// Funktion für das Ermitteln der Start und Zielposition der Motorbewegung (Verschieben der Spielsteine)
Move determineMoveToMove(int Board[3][3], int BoardMemory[3][3], MotorConfig config) {
  Move move;
  int BoardOverlay[3][3];

  // Überlagern der beiden Boards
  overlayBoard(Board, BoardMemory, BoardOverlay);
  
  // Ermitteln der Startposition
  BoardField startField = getChangedField(BoardOverlay, BoardMemory);
  move.startX = config.boardPosition[startField.row][startField.col][0];
  move.startY = config.boardPosition[startField.row][startField.col][1];

  // Ermitteln der Zielposition
  BoardField targetField = getChangedField(BoardOverlay, Board);
  move.targetX = config.boardPosition[targetField.row][targetField.col][0];
  move.targetY = config.boardPosition[targetField.row][targetField.col][1];

  return move;
}

// Funktion für das Ermitteln der Start und Zielpostition der Motorbewegung (Aufräumen der Spielsteine)
Move determineCleanUpMove(int Board[3][3], int garageState[2][5], MotorConfig config) {
  Move move;

  // Finde die Position eines Spielsteins auf dem Spielfeld
  BoardField boardField;
  int stoneType;
  bool found = false;

  for (int i = 0; i < 3 && !found; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != 0) {
        boardField.row = i;
        boardField.col = j;
        stoneType = Board[i][j];
        found = true;  // Markiere als gefunden
        break;         // Innere Schleife verlassen
      }
    }
  }

  // Ermitteln der Startposition
  move.startX = config.boardPosition[boardField.row][boardField.col][0];
  move.startY = config.boardPosition[boardField.row][boardField.col][1];

  // Ermitteln eines freien Platzes und setzen der Zielposition
  if (stoneType == Player1) {

    // Ermitteln der Garageposition
    int garagePosition = findGaragestate(garageState[1], 0);
    garageState[1][garagePosition] = 1;

    // Setzen der Zielposition
    move.targetX = config.playerGaragePosition[garagePosition][0];
    move.targetY = config.playerGaragePosition[garagePosition][1];
  } else {

    // Ermitteln der Garageposition
    int garagePosition = findGaragestate(garageState[0], 0);
    garageState[0][garagePosition] = 1;

    // Setzen der Zielposition
    move.targetX = config.computerGaragePosition[garagePosition][0];
    move.targetY = config.computerGaragePosition[garagePosition][1];
  }
  return move;
}


// ====================================================================================================
// Funktionen für das Zurücksetzen der Spieleinstellungen / des Spielfelds
// ====================================================================================================

// Funktion zum Warten auf das Zurücksetzen des Spielfelds
void awaitBoardReset (int Board[3][3], const int potPins[]) {
  int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  while (!isBoardEqual(Board, ResetBoard)) {
    updateBoard(Board, potPins);
    delay(100);
  }
}

// Funktion zum Warten auf das Zurücksetzen des Spielfelds
void cleanBoard (int Board[3][3], const int potPins[], MotorController &motorController, int garageState[2][5], MotorConfig config) {
  int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  while (!isBoardEqual(Board, ResetBoard)) {
    // Stein aufräumen
    Move move = determineCleanUpMove(Board, garageState, config);
    motorController.moveStone(move);
    delay(2000);
    updateBoard(Board, potPins);
    delay(100);
  }
}

// Funktion zum resetten der Spieleinstellungen / GameSettings
void resetGameSettings(GameSettings &gameSettings) {
  gameSettings.game = 0;
  gameSettings.mode = 0;
  gameSettings.difficulty = 0;
}

#endif