#ifndef TICTACTOE_H
#define TICTACTOE_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "input.h"
#include "game_logic.h"
#include "display.h"

// Konstanten für die Spielmodi
const int PlayerVsComputer = 1;                                                                                   // Konstante für den Spielmodus Spieler gegen Computer
const int PlayerVsPlayer = 2;                                                                                     // Konstante für den Spielmodus Spieler gegen Spieler

// Konstanten für die Spieler
const int Player1 = 1;                                                                                            // Konstante für Spieler 1
const int Player2 = 2;                                                                                            // Konstante für Spieler 2
const int Computer = 2;                                                                                           // Konstante für den Computer

// Konstanten für die Schwierigkeitsgrade
const int Einfach = 1;                                                                                            // Konstante für den Schwierigkeitsgrad Einfach
const int Mittel = 2;                                                                                             // Konstante für den Schwierigkeitsgrad Mittel
const int Schwer = 3;                                                                                             // Konstante für den Schwierigkeitsgrad Schwer

// Funktion für den Spielmodus Tic Tac Toe Spieler gegen Spieler
void TicTacToePlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool firstDisplay = true;                                                                                       // Variable für die erste Anzeige des Spielfelds
  bool turnOver = false;                                                                                          // Variable für den Spielzug
  int turn = 0;                                                                                                   // Zähler für die Spielzüge

  while (true) {
    if (firstDisplay) {                                                                                           // Überprüfen, ob das Spielfeld zum ersten Mal angezeigt wird
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      firstDisplay = false;
    }

    updateBoard(Board, potPins);                                                                                  // Spielfeld aktualisieren
    turnOver = playerMove(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                         // Spielerzug

    if (turnOver) {                                                                                               // Überprüfen, ob der Zug beendet wurde
      turn++;                                                                                                     // Zähler für die Spielzüge erhöhen

      int evaluation = evaluateBoard(Board);                                                                           // Bewertung des Spielfelds
      if (evaluation == 10 || evaluation == -10) {
        displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Player2);                                   // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);                                                                                         // Unentschieden anzeigen
        break;
      }
      copyBoard(Board, BoardMemory);  
      switchPlayer(currentPlayer);                                                                                // Spieler wechseln
      displayPlayer(lcd, gameSettings, currentPlayer);
      turnOver = false;                                                                                           // Zug beenden
    }
  }
}

// Funktion für den Spielmodus Tic Tac Toe Spieler gegen Computer
void TicTacToePlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool firstDisplay = true;
  bool turnOver = false;
  int turn = 0;
  
  // Spiellogik für Tic Tac Toe
  while (true) {
    if (firstDisplay) {
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      firstDisplay = false;
    }

    updateBoard(Board, potPins);
    if (currentPlayer == Player1) {                                                                               // Überprüfen, ob Spieler 1 am Zug ist
                                                                                          
      // Spielerzug
      turnOver = playerMove(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);

    } else if (currentPlayer == Computer) {
      // Computerzug
      int BoardDisplay[3][3];
      copyBoard(Board, BoardDisplay);

      if (gameSettings.difficulty == Einfach || (gameSettings.difficulty == Mittel && turn < 3)) {
        makeRandomMove(BoardDisplay);                                                                             // Zufälligen Zug für den Computer bestimmen
      } else {
        makeBestMove(BoardDisplay);                                                                               // Besten Zug für den Computer bestimmen
      }

      delay(400);
      displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer);                                          // Spielfeld anzeigen
      awaitBoardIsEqual(Board, BoardDisplay, potPins);                                                            // Warten, bis der Computerzug gemacht wurde
      turnOver = true;                                                                                            // Zug beenden                                                                                          // Zug beenden
    }

    if (turnOver) {                                                                                               // Überprüfen, ob der Zug beendet wurde
      turn++;                                                                                                     // Zähler für die Spielzüge erhöhen

      int evaluation = evaluateBoard(Board);                                                                      // Bewertung des Spielfelds
      if (evaluation == 10 || evaluation == -10) {
        displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Computer);                                  // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);                                                                                         // Unentschieden anzeigen
        break;
      }

      copyBoard(Board, BoardMemory);  
      switchPlayer(currentPlayer);                                                                                // Spieler wechseln
      displayPlayer(lcd, gameSettings, currentPlayer);
      turnOver = false;                                                                                           // Zug beenden
    }
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  // Spiellogik für Tic Tac Toe
  if (gameSettings.mode == PlayerVsComputer) {                                         
    TicTacToePlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                     // Spieler gegen Computer
  } else if (gameSettings.mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                       // Spieler gegen Spieler
  }
}

#endif