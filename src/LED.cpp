#include "LED.h"

// Konstruktor
LED::LED(int redPin, int greenPin, int bluePin) 
    : redPin(redPin), greenPin(greenPin), bluePin(bluePin) {}

// Initialisierung der Pins
void LED::begin() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    setRGB(0, 0, 0); // LEDs ausschalten
}

// RGB-Werte setzen
void LED::setRGB(int r, int g, int b) {
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

void LED::setColor(const String& color) {       // RGB-Farbwerte sind invertiert, weil die LED-Pins als Ground verwendet werden
    if (color == "Aus") {
        setRGB(255, 255, 255);
    } else if (color == "Rot") {
        setRGB(0, 255, 255);
    } else if (color == "Gruen") {
        setRGB(255, 0, 255);
    } else if (color == "Blau") {
        setRGB(255, 255, 0);
    } else if (color == "Weiss") {
        setRGB(0, 0, 0);
    } else {
        setRGB(255, 255, 255);
    }
}


