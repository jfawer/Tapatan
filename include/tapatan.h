#ifndef TAPATAN_H
#define TAPATAN_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "input.h"
#include "game_logic.h"
#include "display.h"


// Funktion für den Spielmodus Tapatan Spieler gegen Spieler
void TapatanPlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    bool isInitialDisplay = true;                                                                                               // Flag für die anfängliche Anzeige des Spiels
    bool isTurnOver = false;                                                                                                    // Flag, um den Zug zu überprüfen
    int turnCount = 0;                                                                                                          // Zähler für die Anzahl der Züge

    while (true) {
        if (isInitialDisplay) {                                                                                                 // Überprüfen, ob das Spielfeld zum ersten Mal angezeigt wird
            displayGameScreen(lcd, gameSettings, Board, currentPlayer);
            isInitialDisplay = false;
        }

        int otherPlayer = (currentPlayer == Player1) ? Computer : Player1;                                                      // Anderer Spieler
        updateBoard(Board, potPins);                                                                                            // Spielfeld aktualisieren

        if (turnCount < 6) {                                                                                                    // Zug 1-6
            isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                           // Spielerzug (Platzierung)
        } else if (hasBoardChanged(Board, BoardMemory)) {                                                                       // Zug 7-9
            int currentPlayerStones = countPlayerStones(Board, currentPlayer);                                                  // Zählt die Steine des aktuellen Spielers
            int otherPlayerStones = countPlayerStones(Board, otherPlayer);                                                      // Zählt die Steine des anderen Spielers

            if (otherPlayerStones != 3) {                                                                                       // Wenn der andere Spieler nicht 3 Steine hat
                handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);                               // Handhabung eines illegalen Zugs
            } else if (currentPlayerStones == 2) {                                                                              // Wenn der aktuelle Spieler 2 Steine hat
                isTurnOver = handlePlayerMovement(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);               // Spielerzug (Bewegung)
            }
        }

        if (isTurnOver) {                                                                                                       // Überprüfen, ob der Zug beendet wurde
            turnCount++;
            int evaluation = evaluateBoard(Board);                                                                              // Bewerte das Spielfeld

            if (evaluation == 10 || evaluation == -10) {                                                                        // Überprüfung Gewinner
                displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Computer);
                break;                                                                                                          // Spiel beenden
            }

            copyBoard(Board, BoardMemory);                                                                                      // Kopiere das Spielfeld in das Speicherfeld
            switchPlayer(currentPlayer);                                                                                        // Wechsle den Spieler
            displayPlayer(lcd, gameSettings, currentPlayer);
            isTurnOver = false;
        }
    }
}


// Funktion für den Spielmodus Tapatan Spieler gegen Computer
void TapatanPlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    bool isInitialDisplay = true;                                                                                               // Flag für die anfängliche Anzeige des Spiels
    bool isTurnOver = false;                                                                                                    // Flag, um den Zug zu überprüfen
    int turnCount = 0;                                                                                                          // Zähler für die Anzahl der Züge

    while (true) {
        if (isInitialDisplay) {                                                                                                 // Überprüfen, ob das Spielfeld zum ersten Mal angezeigt wird
            displayGameScreen(lcd, gameSettings, Board, currentPlayer);
            isInitialDisplay = false;
        }

        int otherPlayer = (currentPlayer == Player1) ? Computer : Player1;                                                      // Anderer Spieler
        updateBoard(Board, potPins);                                                                                            // Spielfeld aktualisieren

        if (turnCount < 6) {                                                                                                    // Zug 1-6
            if (currentPlayer == Player1) {                                                                                     // Spielerzug (Platzierung)
                // Spielerzug
                isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
            } else if (currentPlayer == Computer) {                                                                             // Computerzug (Platzierung)
                // Computerzug
                int BoardDisplay[3][3];
                copyBoard(Board, BoardDisplay);

                switch (gameSettings.difficulty) {                                                                              // Schwierigkeitsgrad des Computers
                    case Schwer:
                        makeBestMove(BoardDisplay);                                                                             // Besten Zug für den Computer bestimmen
                        break;
                    case Mittel:
                        if (turnCount >= 3) {
                            makeBestMove(BoardDisplay);                                                                         // Besten Zug für den Computer bestimmen
                        } else {
                            makeRandomMove(BoardDisplay);                                                                       // Zufälligen Zug für den Computer bestimmen
                        }
                        break;
                    default:
                        makeRandomMove(BoardDisplay);                                                                           // Zufälligen Zug für den Computer bestimmen
                        break;
                }

                delay(400);
                displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer);
                awaitBoardIsEqual(Board, BoardDisplay, potPins);                                                                // Warten, bis der Computerzug gemacht wurde
                isTurnOver = true;                                                                                              // Zug beenden
            }
        } else {                                                                                                                // Zug 7-9
            if (currentPlayer == Player1) {                                                                                     // Spielerzug
                if (hasBoardChanged(Board, BoardMemory)) {
                    int currentPlayerStones = countPlayerStones(Board, currentPlayer);                                          // Zählt die Steine des aktuellen Spielers
                    int otherPlayerStones = countPlayerStones(Board, otherPlayer);                                              // Zählt die Steine des anderen Spielers

                    if (otherPlayerStones != 3) {                                                                               // Wenn der andere Spieler nicht 3 Steine hat
                        handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);                       // Handhabung eines illegalen Zugs
                    } else if (currentPlayerStones == 2) {
                        isTurnOver = handlePlayerMovement(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);       // Spielerzug (Bewegung)
                    }
                }
            } else if (currentPlayer == Computer) {                                                                             // Computerzug
                // Computerzug
                int BoardDisplay[3][3];
                copyBoard(Board, BoardDisplay);

                TapatanMakeBestMove(BoardDisplay);                                                                              // Besten Zug für den Computer bestimmen
                delay(400);
                displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer);
                awaitBoardIsEqual(Board, BoardDisplay, potPins);
                isTurnOver = true;                                                                                              // Zug beenden
            }
        }

        if (isTurnOver) {                                                                                                       // Überprüfen, ob der Zug beendet wurde
            turnCount++;

            int evaluation = evaluateBoard(Board);                                                                              // Bewertung des Spielfelds
            if (evaluation == 10 || evaluation == -10) {                                                                        // Überprüfung Gewinner
                displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Computer);
                break;
            }
            copyBoard(Board, BoardMemory);
            switchPlayer(currentPlayer);
            displayPlayer(lcd, gameSettings, currentPlayer);
            isTurnOver = false;                                                                                                 
        }
    }
}

// Funktion für das Spiel Tic Tac Toe
void playTapatan(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    // Spiellogik für Tic Tac Toe
    if (gameSettings.mode == PlayerVsComputer) {
        TapatanPlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                                 // Spieler gegen Computer
    } else if (gameSettings.mode == PlayerVsPlayer) {
        TapatanPlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                                   // Spieler gegen Spieler
    }
}

#endif