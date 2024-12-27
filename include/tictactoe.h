#ifndef TICTACTOE_H
#define TICTACTOE_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "input.h"
#include "game_logic.h"
#include "display.h"

// Funktion für den Spielmodus Tic Tac Toe Spieler gegen Spieler
void TicTacToePlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[], int garageState[2][5]) {
  bool isInitialDisplay = true;                                                                                       // Variable für die erste Anzeige des Spielfelds
  bool isTurnOver = false;                                                                                            // Variable für den Spielzug
  int turnCount = 0;                                                                                                  // Zähler für die Spielzüge

  while (true) {
    if (isInitialDisplay) {                                                                                           // Überprüfen, ob das Spielfeld zum ersten Mal angezeigt wird
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      isInitialDisplay = false;
    }

    updateBoard(Board, potPins);                                                                                      // Spielfeld aktualisieren
    isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                         // Spielerzug

    if (isTurnOver) {                                                                                                 // Überprüfen, ob der Zug beendet wurde
      // Garage künstlich leeren
      if (currentPlayer == Player1) {
        int garagePosition = findGaragestate(garageState[1], 1);                                                        // Garageposition finden
        garageState[1][garagePosition] = 0;                                                                             // Garage leeren                                                                         // Garage leeren
      } else {
        int garagePosition = findGaragestate(garageState[0], 1);                                                        // Garageposition finden
        garageState[0][garagePosition] = 0;                                                                             // Garage leeren
      }

      Serial.print("Spieler 1: Garage: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(garageState[1][i]);
        Serial.print(" ");
      }
      Serial.println();
      Serial.print("Spieler 2: Garage: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(garageState[0][i]);
        Serial.print(" ");
      }
      Serial.println();



      
      turnCount++;                                                                                                    // Zähler für die Spielzüge erhöhen

      int evaluation = evaluateBoard(Board);                                                                          // Bewertung des Spielfelds
      if (evaluation == 10 || evaluation == -10) {
        displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Player2);                                       // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);                                                                                             // Unentschieden anzeigen
        break;
      }
      copyBoard(Board, BoardMemory);  
      switchPlayer(currentPlayer);                                                                                    // Spieler wechseln
      displayPlayer(lcd, gameSettings, currentPlayer);
      isTurnOver = false;                                                                                             // Zug beenden
    }
  }
}

// Funktion für den Spielmodus Tic Tac Toe Spieler gegen Computer
void TicTacToePlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[], MotorController &motorController, int garageState[2][5], MotorConfig config) {
  bool isInitialDisplay = true;
  bool isTurnOver = false;
  int turnCount = 0;
  
  // Spiellogik für Tic Tac Toe
  while (true) {
    if (isInitialDisplay) {
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      isInitialDisplay = false;
    }

    updateBoard(Board, potPins);
    if (currentPlayer == Player1) {                                                                                   // Überprüfen, ob Spieler 1 am Zug ist
                                                                                          
      // Spielerzug
      isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
      
      // Garage künstlich leeren
      int garagePosition = findGaragestate(garageState[1], 1);                                                        // Garageposition finden
      garageState[1][garagePosition] = 0;                                                                             // Garage leeren

    } else if (currentPlayer == Computer) {
      // Computerzug
      int BoardDisplay[3][3];
      copyBoard(Board, BoardDisplay);

      if (gameSettings.difficulty == Einfach || (gameSettings.difficulty == Mittel && turnCount < 3)) {
        makeRandomMove(BoardDisplay);                                                                                 // Zufälligen Zug für den Computer bestimmen
      } else {
        makeBestMove(BoardDisplay);                                                                                   // Besten Zug für den Computer bestimmen
      }
      delay(400);

      // Stein bewegen und platzieren
      displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer);                                              // Spielfeld anzeigen
      Move move = determineMoveToPlace(Board, BoardDisplay, garageState, config);                                     // Bewegung des Motors bestimmen
      Serial.println("Computerzug:");
      Serial.print("Start: ");
      Serial.print(move.startX);
      Serial.print(" ");
      Serial.print(move.startY);
      Serial.print(" Ziel: ");
      Serial.print(move.targetX);
      Serial.print(" ");
      Serial.println(move.targetY);
      motorController.moveStone(move);                                                                                // Spielstein platzieren
      delay(2000);
      awaitBoardIsEqual(Board, BoardDisplay, potPins);                                                                // Warten, bis der Computerzug gemacht wurde
      isTurnOver = true;                                                                                              // Zug beenden                                                                                          // Zug beenden
    }

    if (isTurnOver) {                                                                                                 // Überprüfen, ob der Zug beendet wurde
      turnCount++;                                                                                                    // Zähler für die Spielzüge erhöhen

      int evaluation = evaluateBoard(Board);                                                                          // Bewertung des Spielfelds
      if (evaluation == 10 || evaluation == -10) {
        displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Computer);                                      // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);                                                                                             // Unentschieden anzeigen
        break;
      }

      copyBoard(Board, BoardMemory);  
      switchPlayer(currentPlayer);                                                                                    // Spieler wechseln
      displayPlayer(lcd, gameSettings, currentPlayer);
      isTurnOver = false;                                                                                             // Zug beenden
    }
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[], MotorController &motorController, int garageState[2][5], MotorConfig config) {
  // Spiellogik für Tic Tac Toe
  if (gameSettings.mode == PlayerVsComputer) {                                         
    TicTacToePlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins, motorController, garageState, config); // Spieler gegen Computer
  } else if (gameSettings.mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins, garageState);                                         // Spieler gegen Spieler
  }
}

#endif