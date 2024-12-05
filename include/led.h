#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
private:
    int redPin, greenPin, bluePin;

    // Private Methode zum Setzen der RGB-Werte
    void setRGB(int r, int g, int b);

public:
    // Konstruktor
    LED(int redPin, int greenPin, int bluePin);

    // Initialisierungsmethode
    void begin();

    // Methode zum Setzen der Farbe
    void setColor(const String& color);
};

#endif