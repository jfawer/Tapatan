#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <AccelStepper.h>
#include <MultiStepper.h>
#include "struct.h"

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

    // Motor-Konfiguration
    MotorConfig config;

    // Konstanten der Schrittmotoren
    const float stepsPerRevolution = 800.0; // Schritte pro Umdrehung
    const float diameter = 10.0;            // Durchmesser des Riemenantriebs in mm

    // Private Hilfsfunktionen
    float calculateSteps(float delta);
    void setPosition(int x, int y);
    int determineShortestLane(int col, const int axisLane[2]) const;
    bool isSameLane(int startCol, int targetCol, const int axisLane[2]) const;
    bool isInGarage(int row, int col, const int garagePosition[5][2]) const;

public:
    // Konstruktor
    MotorController(int m1Step, int m1Dir, int m1Enable, int m2Step, int m2Dir, int m2Enable, int esX, int esY, int em, int emPolarity);

    // Setup-Funktion für Motoren
    void initialize();

    // Motorsteuerungsfunktionen
    void moveToPosition(int x, int y);
    void enableMotors();
    void disableMotors();
    void homeMotors();
    void moveStone(Move move);

    // Setter für die gesamte Konfiguration
    void setConfig(const MotorConfig& newConfig);

    // Getter für aktuelle Position
    int getCurrentXPosition() const { return currentXPosition; }
    int getCurrentYPosition() const { return currentYPosition; }
};

#endif