/*
  Projekt: Tapatan und Tic-Tac-Toe
  Mitwirkende: Eliane Hess, Aymeric Nouvier, Simeon Yasmin, Liam Mahieu, Jan Fawer
  Version: 1.0
*/

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "LED.h"
#include "MotorController.h"
#include "tictactoe.h"
#include "tapatan.h"

// --------------------------------------------------------------------------------
// Rotary Encoder
// --------------------------------------------------------------------------------
bool setupvariable;                                                                                       // Variable für das Aktivieren des Encoders
volatile int rotarySwitchValue = 0;                                                                       // Aktueller Wert des Encoders
const int rotarySwitchPin1 = 2;                                                                           // Pin für den Rotary-Encoder (CLK)
const int rotarySwitchPin2 = 25;                                                                          // Pin für den Rotary-Encoder (DT)
const int gameButtonPin = 24;                                                                             // Pin für den Bestätigungsknopf (SW)

// Interrupt Service Routine (ISR)
void handleEncoder() {
  if (setupvariable) {                                                                                    // Überprüfen, ob die Setupvariable gesetzt ist
    getRotaryValue(rotarySwitchPin1, rotarySwitchPin2, rotarySwitchValue);                                // Encoderwert auslesen
  }
}

// --------------------------------------------------------------------------------
// LED  Streifen
// --------------------------------------------------------------------------------

LED led(13, 22, 12);                                                                                      // LED Streifen

// --------------------------------------------------------------------------------
// LCD Display
// --------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x27, 20, 21);                                                                      // Setze die Adresse des LCD-Displays
byte umlautU[8] = { B01010, B00000, B10001, B10001, B10001, B10011, B01101, B00000 };                     // Umlaut ü
byte umlautO[8] = { B01010, B00000, B01110, B10001, B10001, B10001, B01110, B00000 };                     // Umlaut ö
byte umlautA[8] = { B01010, B00000, B01110, B00001, B01111, B10001, B01111, B00000 };                     // Umlaut ä

// --------------------------------------------------------------------------------
// Motorsteuerung
// --------------------------------------------------------------------------------

// Definieren des Motorcontrollers
MotorController motorController(23, 3, 4, 5, 6, 7, 8, 17, 10, 11);                                        // Motorcontroller
MotorConfig config;                                                                                       // Konfiguration des Motorcontrollers

// --------------------------------------------------------------------------------
// Sensorik / Spielsteinerkennung
// --------------------------------------------------------------------------------

// Pins für die Sensorik
const int potPins[] = {A3, A2, A1, A6, A5, A4, A9, A8, A7}; 

// --------------------------------------------------------------------------------
// Konstanten und Variablen
// --------------------------------------------------------------------------------

// Spieleinstellungen
GameSettings gameSettings = {                                                           
  .game = 0,                                                                                              // Spiel / 1: Tic Tac Toe, 2: Tapatan
  .mode = 0,                                                                                              // Spielmodus / 1: Spieler gegen Computer, 2: Spieler gegen Spieler
  .difficulty = 0,                                                                                        // Schwierigkeitsgrad / 1: Einfach, 2: Mittel, 3: Schwer
};
                                              
// Konstanten für die Spiele
const int TicTacToe = 1;                                                                                  // Konstante für das Spiel Tic Tac Toe
const int Tapatan = 2;                                                                                    // Konstante für das Spiel Tapatan

int Board [3][3];                                                                                         // Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)
int BoardMemory [3][3];                                                                                   // Array für das den Spielfeldspeicher (0 = Leeres Feld, 1 = X, 2 = O)
int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};                                                         // Array für das Zurücksetzen des Spielfelds
int currentPlayer;                                                                                        // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer
int garageState[2][5] = {{1,1,1,1,1},{1,1,1,1,1}};                                                        // Array für die Garagen (0 = Leer, 1 = Belegt) / Computer, Spieler

// Leerer Pin für die Zufallsfunktion
const int emptyAnalogPin = A10;                                                                           // Pin für den Seed der Zufallsfunktion

// --------------------------------------------------------------------------------
// Setup Funktion
// --------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);                                                                                     // Serielle Kommunikation starten
  
  // Initialisierung der Pins
  pinMode(rotarySwitchPin1, INPUT_PULLUP);                                                                // Pin für den Rotary-Encoder (CLK) als Eingang
  pinMode(rotarySwitchPin2, INPUT_PULLUP);                                                                // Pin für den Rotary-Encoder (DT) als Eingang
  pinMode(gameButtonPin, INPUT_PULLUP);                                                                   // Pin für den Bestätigungsknopf (SW) als Eingang
  
  // Initialisierung des LED Streifens
  led.begin();                                                                                            // LED Streifen initialisieren
  led.setColor("Lila");                                                                                 // LED Streifen auf orange setzen

  // Initialisierung des LCD-Displays
  lcd.init();                                                                                             // LCD initialisieren
  lcd.createChar(0, umlautU);                                                                             // Umlaut Ü erstellen
  lcd.createChar(1, umlautO);                                                                             // Umlaut Ö erstellen
  lcd.createChar(2, umlautA);                                                                             // Umlaut Ä erstellen
  lcd.backlight();                                                                                        // Hintergrundbeleuchtung einschalten
  displayStart(lcd);                                                                                      // Startbildschirm anzeigen
  delay(2000);

  // Initialisierung des Motorcontrollers
  motorController.initialize();                                                                           // Motorcontroller initialisieren
  motorController.setConfig(config);                                                                      // Konfiguration des Motorcontrollers setzen
  motorController.homeMotors();                                                                           // Motoren in die Home-Position fahren

  // Initialisierung des Spiels
  setupvariable = false;                                                                                  // Setupvariable auf false setzen
  attachInterrupt(digitalPinToInterrupt(rotarySwitchPin1), handleEncoder, CHANGE);                        // Interrupt für den Rotary-Encoder einrichten
  randomSeed(analogRead(emptyAnalogPin));                                                                 // Seed für die Zufallsfunktion

  // Initialisierung des Spielfelds
  updateBoard(Board, potPins);                                                                            // Sensorwerte auslesen
  if (!isBoardEqual(Board, ResetBoard)) {                                                                 // Überprüfen, ob das Spielfeld nicht in der Ausgangsposition ist
    displayReset(lcd);                                                                                    // Anzeige zum Zurücksetzen des Spielfelds
    awaitBoardReset(Board, potPins);                                                                      // Warten auf das Zurücksetzen des Spielfelds
    resetGameSettings(gameSettings);                                                                      // Spieleinstellungen zurücksetzen
    copyBoard(ResetBoard, BoardMemory);                                                                   // Spielfeldspeicher zurücksetzen
  }

  // Initaliserung abgeschlossen
  led.setColor("Gruen");                                                                                  // LED Streifen auf grün setzen
}

// --------------------------------------------------------------------------------
// Loop Funktion
// --------------------------------------------------------------------------------

void loop() {
  // Spiellogik
  updateBoard(Board, potPins);                                                                            // Sensorwerte auslesen
  if (isBoardEqual(Board, ResetBoard) && gameSettings.game == 0) {                                        // Überprüfen, ob das Spielfeld in der Ausgangsposition ist und kein Spiel ausgewählt wurde
    // Spiel auswählen
    choseGameSettings(lcd, gameSettings, gameButtonPin, setupvariable);                                   // Spieleinstellungen auswählen
    currentPlayer = random(1, 3);                                                                         // Zufällige Auswahl des Startspielers
    delay(500);

    // Spiel starten
    switch (gameSettings.game) {                                                                          // Auswahl des Spiels
      case TicTacToe:
        led.setColor("Zyan");                                                                             // LED Streifen auf zyan setzen
        playTicTacToe(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins, motorController, garageState, config); // Spiel Tic Tac Toe starten
        delay(5000);
        break;
      case Tapatan:
        led.setColor("Weiss");                                                                            // LED Streifen auf weiss setzen
        playTapatan(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins, garageState, motorController, config); // Spiel Tapatan starten
        delay(5000);
        break;
    }
  } else {
    led.setColor("Lila");                                                                                 // LED Streifen auf lila setzen
    displayReset(lcd);                                                                                    // Anzeige zum Zurücksetzen des Spielfelds
    cleanBoard(Board, potPins, motorController, garageState, config);                                     // Spielfeld reinigen
    awaitBoardReset(Board, potPins);                                                                      // Warten auf das Zurücksetzen des Spielfeld
    resetGameSettings(gameSettings);                                                                      // Spieleinstellungen zurücksetzen
    resetGarageState(garageState);                                                                        // Garage zurücksetzen
    copyBoard(ResetBoard, BoardMemory);                                                                   // Spielfeldspeicher zurücksetzen
  }
  
  /*
  // Serielle Eingabe einer X- und Y-Position
  int x, y;
  Serial.println("X-Position:");
  while (Serial.available() == 0) {}
  x = Serial.parseInt();
  Serial.println("Y-Position:");
  while (Serial.available() == 0) {}
  y = Serial.parseInt();
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.println(y);

  // Motorsteuerung
  motorController.moveToPosition(x, y);
  delay(2000);
  */
}