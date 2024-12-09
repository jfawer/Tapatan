#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <AccelStepper.h>
#include <MultiStepper.h>

class MotorController {
private:
    // Pins für Motorsteuerung
    const int motor1StepPin, motor1DirPin, motor1EnablePin;
    const int motor2StepPin, motor2DirPin, motor2EnablePin;

    // Pins für Endschalter und Elektromagnet
    const int endstopXPin, endstopYPin;
    const int electromagnetPin, electromagnetPolarityPin;

    // Motor-Objekte
    AccelStepper Motor1;
    AccelStepper Motor2;
    MultiStepper Motoren;

    // Aktuelle Positionen
    int currentXPosition, currentYPosition;

    // Maximal- und Minimalpositionen
    const int maxXPosition, maxYPosition;
    const int minXPosition, minYPosition;

    // Konstanten der Schrittmotoren
    const float stepsPerRevolution = 800.0; // Schritte pro Umdrehung
    const float diameter = 10.0;            // Durchmesser des Riemenantriebs in mm

    // Private Hilfsfunktionen
    float calculateSteps(float delta);
    void setPosition(int x, int y);

public:
    // Konstruktor
    MotorController(int m1Step, int m1Dir, int m1Enable, int m2Step, int m2Dir, int m2Enable, int esX, int esY, int em, int emPolarity, int maxX, int maxY, int minX, int minY);

    // Setup-Funktion für Motoren
    void initialize();

    // Motorsteuerungsfunktionen
    void moveToPosition(int x, int y);
    void enableMotors();
    void disableMotors();
    void homeMotors();

    // Getter für aktuelle Position
    int getCurrentXPosition() const { return currentXPosition; }
    int getCurrentYPosition() const { return currentYPosition; }
};

#endif