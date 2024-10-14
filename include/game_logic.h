// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "input.h"

const int PlayerVsPlayer = 1;                                           // Konstante für den Spielmodus Spieler gegen Spieler
const int PlayerVsComputer = 2;                                         // Konstante für den Spielmodus Spieler gegen Computer


// Funktion zum Zeichnen Spielfelds
void drawGameBoard(LiquidCrystal_I2C &lcd, int Board[3][3], int game, int mode, int turn) {

  lcd.clear();                                                         // LCD löschen

  // Zeige den Spieltyp auf dem LCD an
  lcd.setCursor(0, 0);
  lcd.print(game == 1 ? "Tic Tac Toe:" : "Tapatan:");

  // Zeige den Spieler, der am Zug ist
  lcd.setCursor(0, 2);
  lcd.print("Am Zug:");
  
  lcd.setCursor(0, 3);
  if (mode == 1) {
    lcd.print(turn == 1 ? "Spieler" : "Computer");
  } else {
    lcd.print(turn == 1 ? "Spieler 1" : "Spieler 2");
  }
  
  // Zeichne das Spielfeld auf dem LCD
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      lcd.setCursor(col + 13, row + 1);
      
      // Entscheide, welches Symbol angezeigt werden soll
      char symbol = (Board[row][col] == 0) ? '0' : (Board[row][col] == 1) ? '1' : '2';
      lcd.print(symbol);
    }
  }
}

// Funktion für das Spiel Tic Tac Toe Spieler gegen Spieler
void TicTacToePlayerVsPlayer(LiquidCrystal_I2C lcd, int Board[3][3], int game, int mode, int &turn) {
  static int turnNumber = 1; // Zähler für die Anzahl der Züge

  if (turnNumber <= 4) {
    // Spieler ist am Zug
    
    // Spieler wechseln
    turn = (turn == 1) ? 2 : 1;
    drawGameBoard(lcd, Board, game, mode, turn);

    // Zähler erhöhen
    turnNumber++;
  } else {
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C lcd, int Board[3][3], int game, int mode, int &turn, int difficulty) {
  // Spiellogik für Tic Tac Toe
  if (mode == PlayerVsComputer) {                                         
    // Spieler gegen Computer
  } else if (mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, Board, game, mode, turn);      // Spieler gegen Spieler
  }
}




  /*
  if (turn == 1) {                                           // Der Spieler ist am Zug                   
    boolean playerInput = false;

    while (!playerInput) {                                   // Eine Schleife, bis der Spieler einen gültigen Zug gemacht hat
      for (int i=0; i<=2 ; i++)                              
        for (int j=0; j<=2; j++)
          if (digitalRead(buttons[i][j]) == HIGH)            
          {
            if (Board [i][j] == 0)                       // Überprüfen, ob das Feld leer ist
            {
              Board[i][j] = 1;                           // Das Feld auf dem Spielfeld hinzufügen und den Zug wechseln
              turn = 2;
              playerInput = true;
            }
          }
    }

    drawGameBoard ();                                         // Das Spielfeld anzeigen                  
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
              if (Board [i][j] == 0) {                   // Überprüfen, ob das Feld leer ist
                Board[i][j] = 2;                         // Das Feld auf dem Spielfeld hinzufügen und den Zug wechseln
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
  int winner = evaluate (Board);                         // Überprüfen, ob ein Spieler gewonnen hat
  if (winner == 10 || winner == -10)                         // Wenn der Schleifenwert 10 oder -10 ist, hat ein Spieler gewonnen
  {
    flashWin () ;                                            // Gewinnanimation
    resetBoard ();                                           // Das Spielfeld zurücksetzen              
    startGameAni ();                                         // Startanimation              
    updateDisplay ();                                        // Das Spielfeld aktualisieren      
  }
  if (!checkMovesLeft(Board))                            // Überprüfen, ob das Spielfeld voll ist und es ein Unentschieden gibt
  {
    flashDraw();                                             // Unentschiedenanimation        
    resetBoard ();                                           // Das Spielfeld zurücksetzen    
    startGameAni ();                                         // Startanimation    
    updateDisplay ();                                        // Das Spielfeld aktualisieren  
  }
  */

#endif
