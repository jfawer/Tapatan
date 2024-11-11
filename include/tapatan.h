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
    bool isInitialDisplay = true;
    bool isTurnOver = false;
    int turnCount = 0;

    while (true) {
        if (isInitialDisplay) {
            displayGameScreen(lcd, gameSettings, Board, currentPlayer);
            isInitialDisplay = false;
        }

        int otherPlayer = (currentPlayer == Player1) ? Computer : Player1;
        updateBoard(Board, potPins);

        if (turnCount < 6) {
            isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
        } else if (hasBoardChanged(Board, BoardMemory)) {
            int currentPlayerStones = countPlayerStones(Board, currentPlayer);
            int otherPlayerStones = countPlayerStones(Board, otherPlayer);

            if (otherPlayerStones != 3) {
                handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);
            } else if (currentPlayerStones == 2) {
                isTurnOver = handlePlayerMovement(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
            }
        }

        if (isTurnOver) {
            turnCount++;
            int evaluation = evaluateBoard(Board);
            if (evaluation == 10 || evaluation == -10) {
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

// Funktion für den Spielmodus Tapatan Spieler gegen Computer
void TapatanPlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    bool isInitialDisplay = true;
    bool isTurnOver = false;
    int turnCount = 0;

    while (true) {
        if (isInitialDisplay) {
            displayGameScreen(lcd, gameSettings, Board, currentPlayer);
            isInitialDisplay = false;
        }

        int otherPlayer = (currentPlayer == Player1) ? Computer : Player1;
        updateBoard(Board, potPins);

        if (turnCount < 6) {
            if (currentPlayer == Player1) {
                // Spielerzug
                isTurnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
            } else if (currentPlayer == Computer) {
                // Computerzug
                int BoardDisplay[3][3];
                copyBoard(Board, BoardDisplay);
                
                makeBestMove(BoardDisplay); // Besten Zug für den Computer bestimmen

                delay(400);
                displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer); // Spielfeld anzeigen
                awaitBoardIsEqual(Board, BoardDisplay, potPins); // Warten, bis der Computerzug gemacht wurde
                isTurnOver = true; // Zug beenden
            }
        } else {
            if (currentPlayer == Player1) {
                if (hasBoardChanged(Board, BoardMemory)) {
                    int currentPlayerStones = countPlayerStones(Board, currentPlayer);
                    int otherPlayerStones = countPlayerStones(Board, otherPlayer);

                    if (otherPlayerStones != 3) {
                        handleIllegalMove(lcd, Board, BoardMemory, potPins, gameSettings, currentPlayer);
                    } else if (currentPlayerStones == 2) {
                        isTurnOver = handlePlayerMovement(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
                    }
                }
            } else if (currentPlayer == Computer) {
                // Computerzug
                int BoardDisplay[3][3];
                copyBoard(Board, BoardDisplay);

                TapatanmakeBestMove(BoardDisplay); // Besten Zug für den Computer bestimmen
                delay(400);
                displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer); // Spielfeld anzeigen
                awaitBoardIsEqual(Board, BoardDisplay, potPins); // Warten, bis der Computerzug gemacht wurde
                isTurnOver = true; // Zug beenden
            }
        }

        if (isTurnOver) {
            turnCount++;

            int evaluation = evaluateBoard(Board);
            if (evaluation == 10 || evaluation == -10) {
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
        TapatanPlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                       // Spieler gegen Computer
    } else if (gameSettings.mode == PlayerVsPlayer) {
        TapatanPlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                         // Spieler gegen Spieler
    }
}

#endif