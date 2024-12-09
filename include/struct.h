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

// Konstanten für die Farben
const int Aus = 0;                                                                // Konstante für die Farbe aus
const int Rot = 1;                                                                // Konstante für die Farbe rot
const int Gruen = 2;                                                              // Konstante für die Farbe grün
const int Blau = 3;                                                               // Konstante für die Farbe blau
const int Weiss = 4;                                                              // Konstante für die Farbe weiss

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
  int startX;                                                                   // Start X-Position des Motors
  int startY;                                                                   // Start Y-Position des Motors
  int targetX;                                                                  // Ziel X-Position des Motors
  int targetY;                                                                  // Ziel Y-Position des Motors
};

struct MotorConfig {
  int verticalLanePositions[2] = {75, 225};                    
  int horizontalLanePositions[2] = {120, 180};                 

  int computerGaragePosition[5][2] = {                         
    {20, 50},
    {80, 50},
    {140, 50},
    {200, 50},
    {260, 50}
  };

  int playerGaragePosition[5][2] = {                           
    {20, 350},
    {80, 350},
    {140, 350},
    {200, 350},
    {260, 350}
  };

  int boardPosition[3][3][2] = {                               
    {{80, 100}, {80, 150}, {80, 200}},
    {{140, 100}, {140, 150}, {140, 200}},
    {{200, 100}, {200, 150}, {200, 200}}
  };

  int maxXPosition = 315;                                      
  int maxYPosition = 405;                                      
  int minXPosition = 0;                                        
  int minYPosition = 0;                                        
};

#endif