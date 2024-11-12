// Struktur für den Spielzustand und die Felder des Spielfelds

#ifndef STRUCT_H
#define STRUCT_H

#include <Arduino.h>

// Konstanten für die Spielmodi
const int PlayerVsComputer = 1;                                                   // Konstante für den Spielmodus Spieler gegen Computer
const int PlayerVsPlayer = 2;                                                     // Konstante für den Spielmodus Spieler gegen Spieler

// Konstanten für die Spieler
const int Player1 = 1;                                                            // Konstante für Spieler 1
const int Player2 = 2;                                                            // Konstante für Spieler 2
const int Computer = 2;                                                           // Konstante für den Computer

// Konstanten für die Schwierigkeitsgrade
const int Einfach = 1;                                                            // Konstante für den Schwierigkeitsgrad Einfach
const int Mittel = 2;                                                             // Konstante für den Schwierigkeitsgrad Mittel
const int Schwer = 3;                                                             // Konstante für den Schwierigkeitsgrad Schwer


// Struktur für den Spielzustand
struct GameSettings {
  int game;                                                                       // Variable für das Spiel 1 = Tic Tac Toe, 2 = Tapatan
  int mode;                                                                       // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
  int difficulty;                                                                 // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Mittel, 3 = Schwer
};

// Struktur für ein Feld des Spielfelds
struct BoardField {
    int row;                                                                      // Zeile des Feldes
    int col;                                                                      // Spalte des Feldes
};

#endif