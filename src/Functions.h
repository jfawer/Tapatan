#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Funktion zum Zeichnen Spielfelds
void displayBoard(LiquidCrystal_I2C &lcd, int gameBoard[3][3], int gameType, int gameMode, int turn) {

  lcd.clear();                                                          // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print(gameType == 1 ? "Tic Tac Toe:" : "Tapatan:");                 // Zeige den Spieltyp auf dem LCD an

  lcd.setCursor(0, 2);
  lcd.print("Am Zug:");                                                 // Zeige den Spieler an der Reihe auf dem LCD an                         
  lcd.setCursor(0, 3);
  if (gameMode == 1) {
    lcd.print(turn == 1 ? "Spieler" : "Computer");                                    
  } else {
    lcd.print(turn == 1 ? "Spieler 1" : "Spieler 2");
  }
  
  // Zeichne das Spielfeld auf dem LCD
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      lcd.setCursor(col+13, row+1);

      // Entscheidet, welches Symbol angezeigt werden soll
      if (gameBoard[row][col] == 0) {
        lcd.print("0");     // Leeres Feld
      } else if (gameBoard[row][col] == 1) {
        lcd.print("1");     // X anzeigen
      } else if (gameBoard[row][col] == 2) {
        lcd.print("2");     // O anzeigen
      }
    }
  }
}

// Funktion zum Eingeben der Spielvariablen
void inputGameVariables(int &gameType, int &gameMode, int &difficulty, int &turn) {
  String input;

  // Spieltyp eingeben
  Serial.println("Gib den Spieltyp ein (1 = Tic Tac Toe, 2 = Tapatan):");
  while (Serial.available() == 0) {}                                        // Warten auf Eingabe
  gameType = Serial.parseInt();                                             // Lesen der Eingabe
  Serial.print("Gewählter Spieltyp: ");
  Serial.println(gameType);

  // Spielmodus eingeben
  Serial.println("Gib den Spielmodus ein (1 = Spieler gegen Computer, 2 = Spieler gegen Spieler):");
  while (Serial.available() == 0) {}                                        // Warten auf Eingabe
  gameMode = Serial.parseInt();                                             // Lesen der Eingabe
  Serial.print("Gewählter Spielmodus: ");
  Serial.println(gameMode);

  // Schwierigkeitsgrad eingeben (nur bei Spieler gegen Computer)
  if (gameMode == 1) {
    Serial.println("Gib den Schwierigkeitsgrad ein (1 = Einfach, 2 = Schwer):");
    while (Serial.available() == 0) {}                                      // Warten auf Eingabe
    difficulty = Serial.parseInt();                                         // Lesen der Eingabe
    Serial.print("Gewählter Schwierigkeitsgrad: ");
    Serial.println(difficulty);
  } else {
    difficulty = 0;                                                         // Setzt die Schwierigkeit auf 0, wenn Spieler gegen Spieler gespielt wird
  }

  // Zufällige Auswahl des Startspielers
  randomSeed(analogRead(0));                                                // Initialisieren des Zufallsgenerators                    
  turn = random(1,3);                                                       // Zufällige Auswahl des Startspielers (1 oder 2)
}

// Funktion zum Eingeben des Spielfelds
void inputGameBoard(int gameBoard[3][3]) {
  Serial.println("Gib die Werte für das Spielfeld ein (0 = Leeres Feld, 1 = X, 2 = O):");

  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      Serial.print("Wert für Feld [");
      Serial.print(row);
      Serial.print("][");
      Serial.print(col);
      Serial.println("]:");

      // Warten auf Eingabe
      while (Serial.available() == 0) {}  // Warten, bis eine Eingabe vorhanden ist
      
      // Liest den Wert und konvertiert ihn in einen Integer
      gameBoard[row][col] = Serial.parseInt();
      Serial.print("Eingegebener Wert: ");
      Serial.println(gameBoard[row][col]);
    }
  }

  // Optional: Anzeige des gesamten eingegebenen Arrays
  Serial.println("Eingegebenes Spielfeld:");
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      Serial.print(gameBoard[row][col]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

// Funktion für das Spiel Tic Tac Toe
void TicTacToe(int gameBoard[3][3], int gameMode, int turn, int difficulty) {
  // Spiellogik für Tic Tac Toe
  if (gameMode == 1) {                                         // Spieler gegen Computer
    // Code für Spieler gegen Computer
  } else {                                                     // Spieler gegen Spieler
    // Code für Spieler gegen Spieler
  }

  /*
  if (turn == 1) {                                           // Der Spieler ist am Zug                   
    boolean playerInput = false;

    while (!playerInput) {                                   // Eine Schleife, bis der Spieler einen gültigen Zug gemacht hat
      for (int i=0; i<=2 ; i++)                              
        for (int j=0; j<=2; j++)
          if (digitalRead(buttons[i][j]) == HIGH)            
          {
            if (gameBoard [i][j] == 0)                       // Überprüfen, ob das Feld leer ist
            {
              gameBoard[i][j] = 1;                           // Das Feld auf dem Spielfeld hinzufügen und den Zug wechseln
              turn = 2;
              playerInput = true;
            }
          }
    }

    displayBoard ();                                         // Das Spielfeld anzeigen                  
    Serial.println("");

  } else {                                                   // Der Computer ist am Zug                
    if (mode == 0) {                                         // Schwierigkeitsgrad 2 - Der Computer macht perfekte Züge
      delay(300);                                            
      aITurn ();                                             // Perfekter Zug des Computers

    } else if (mode == 1) {                                  // Schwierigkeitsgrad 1 - Der Computer macht zufällige Züge
      delay(500);                                            
      randomTurn();                                          // Zufälliger Zug des Computers

    } else {                                                 // Spielmodus 2 - Spieler gegen Spieler
      boolean playerInput = false;

      while (!playerInput) {                                 // Eine Schleife, bis der Spieler einen gültigen Zug gemacht hat
        for (int i=0; i<=2 ; i++) {                          
          for (int j=0; j<=2; j++) {
            if (digitalRead(buttons[i][j]) == HIGH) {        
              if (gameBoard [i][j] == 0) {                   // Überprüfen, ob das Feld leer ist
                gameBoard[i][j] = 2;                         // Das Feld auf dem Spielfeld hinzufügen und den Zug wechseln
                turn = 1;
                playerInput = true;
              }
            }
          }
        }
      }
    }
    displayBoard ();                                         // Das Spielfeld anzeigen                  
    Serial.println("");
  }
  */

  /*
  updateDisplay ();                                          // Das Spielfeld aktualisieren
  int winner = evaluate (gameBoard);                         // Überprüfen, ob ein Spieler gewonnen hat
  if (winner == 10 || winner == -10)                         // Wenn der Schleifenwert 10 oder -10 ist, hat ein Spieler gewonnen
  {
    flashWin () ;                                            // Gewinnanimation
    resetBoard ();                                           // Das Spielfeld zurücksetzen              
    startGameAni ();                                         // Startanimation              
    updateDisplay ();                                        // Das Spielfeld aktualisieren      
  }
  if (!checkMovesLeft(gameBoard))                            // Überprüfen, ob das Spielfeld voll ist und es ein Unentschieden gibt
  {
    flashDraw();                                             // Unentschiedenanimation        
    resetBoard ();                                           // Das Spielfeld zurücksetzen    
    startGameAni ();                                         // Startanimation    
    updateDisplay ();                                        // Das Spielfeld aktualisieren  
  }
  */
}

#endif
