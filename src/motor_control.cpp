// Einbinden der Bibliotheken
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "display.h"
#include "input.h"
#include "tictactoe.h"
#include "tapatan.h"
#include "motor.h"



int playerGaragePosition[5][2] = {                                                                                  // Positionen der Garagen des Spielers
    {34, 25}, 
    {34, 20}, 
    {34, 15}, 
    {34, 10},
    {34, 5}
};

int computerGaragePosition[5][2] = {                                                                                // Positionen der Garagen des Computers
    {6, 25}, 
    {6, 20},
    {6, 15}, 
    {6, 10}, 
    {6, 5}
};                                                                                                                  

int BoardPosition[3][3][2] = {                                                                                      // Positionen der Spielfelder
    {{14, 20}, {20, 20}, {26, 20}}, 
    {{14, 15}, {20, 15}, {26, 15}}, 
    {{14, 10}, {20, 10}, {26, 10}}
};        

int laneAxisPosition[2] = {12, 17};                                                                                 // Array für die Achsen der Fahrbahnen

int playerGarage[5] = {0, 0, 0, 0, 0};                                                                              // Status der Garagen des Spielers
int computerGarage[5] = {1, 0, 1, 0, 0};                                                                            // Status der Garagen des Computers

int Board [3][3] = {{0,0,2},{0,2,2},{0,1,0}};                                                                       // Spielfeld
int BoardMemory [3][3] = {{0,2,1},{1,2,2},{0,1,0}};                                                                 // Speicherfeld


void setup() {
    Serial.begin(9600);

    int lane;

    lane = determineShortestDistance(5, laneAxisPosition);
    Serial.print("Lane-1: ");
    Serial.println(lane);

    lane = determineShortestDistance(25, laneAxisPosition);
    Serial.print("Lane-2: ");
    Serial.println(lane);

    lane = determineShortestDistance(13.5, laneAxisPosition);
    Serial.print("Lane-3: ");
    Serial.println(lane);
}


void loop() {

}