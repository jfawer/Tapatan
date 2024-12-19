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

// Konstanten für die Garage
const int Leer = 0;                                                               // Konstante für die Garage leer
const int Belegt = 1;                                                             // Konstante für die Garage belegt

// Struktur für den Spielzustand
struct GameSettings {
  int game;                                                                       // Variable für das Spiel 1 = Tic Tac Toe, 2 = Tapatan
  int mode;                                                                       // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
  int difficulty;                                                                 // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Mittel, 3 = Schwer
};

// Struktur für ein Feld des Spielfelds
struct BoardField {
  int row;                                                                        // Zeile des Feldes
  int col;                                                                        // Spalte des Feldes
};

// Struktur für die Bewegung des Motors
struct Move {
  int startX;                                                                     // Start X-Position des Motors
  int startY;                                                                     // Start Y-Position des Motors
  int targetX;                                                                    // Ziel X-Position des Motors
  int targetY;                                                                    // Ziel Y-Position des Motors
};

// Struktur für die Konfiguration der Motorsteuerung
struct MotorConfig {
  // Positionen der Fahrbahnen
  int verticalLanePositions[2] = {50, 320};                                     
  int horizontalLanePositions[2] = {99, 208};                                 

  // Positionen der Computer-Garagen
  int computerGaragePosition[5][2] = {
    {263, 370},
    {208, 370},
    {154, 370},
    {99, 370},
    {45, 370}
  };

  // Positionen der Spieler-Garagen
  int playerGaragePosition[5][2] = { 
    {263, 20},
    {208, 20},
    {154, 20},
    {99, 20},
    {45, 20}
  };

  // Positionen der Spielfelder
  int boardPosition[3][3][2] = {              
    {{243, 290}, {243, 190}, {243, 90}},
    {{143, 290}, {143, 190}, {143, 90}},
    {{43, 290}, {43, 190}, {43, 90}}
  };

  // Spielfeldbegrenzungen
  int maxXPosition = 315;                                      
  int maxYPosition = 405;                                      
  int minXPosition = 0;                                        
  int minYPosition = 0;                                        
};

#endif