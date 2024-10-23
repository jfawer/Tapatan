// Struktur für den Spielzustand und die Felder des Spielfelds

#ifndef STRUCT_H
#define STRUCT_H

#include <Arduino.h>

// Struktur für den Spielzustand
struct GameSettings {
  int game;                                                                       // Variable für das Spiel 1 = Tic Tac Toe, 2 = Tapatan
  int mode;                                                                       // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
  int difficulty;                                                                 // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Schwer
};

// Struktur für die Felder des Spielfelds
struct BoardField {
    int row;                                                                      // Zeile des Feldes
    int col;                                                                      // Spalte des Feldes
};

#endif