
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "MotorController.h"
#include "struct.h"

MotorController::MotorController(int m1Step, int m1Dir, int m1Enable, int m2Step, int m2Dir, int m2Enable, int esX, int esY, int em, int emPolarity, int maxX, int maxY, int minX, int minY)
    : motor1StepPin(m1Step), motor1DirPin(m1Dir), motor1EnablePin(m1Enable),
      motor2StepPin(m2Step), motor2DirPin(m2Dir), motor2EnablePin(m2Enable),
      endstopXPin(esX), endstopYPin(esY), electromagnetPin(em), electromagnetPolarityPin(emPolarity),
      maxXPosition(maxX), maxYPosition(maxY), minXPosition(minX), minYPosition(minY),
      Motor1(AccelStepper::DRIVER, m1Step, m1Dir), Motor2(AccelStepper::DRIVER, m2Step, m2Dir), Motoren() {
    currentXPosition = 0;
    currentYPosition = 0;
}

void MotorController::initialize() {
    // Initialisierung der Pins
    pinMode(motor1StepPin, OUTPUT);
    pinMode(motor1DirPin, OUTPUT);
    pinMode(motor1EnablePin, OUTPUT);
    pinMode(motor2StepPin, OUTPUT);
    pinMode(motor2DirPin, OUTPUT);
    pinMode(motor2EnablePin, OUTPUT);
    pinMode(endstopXPin, INPUT_PULLUP);
    pinMode(endstopYPin, INPUT_PULLUP);

    // Konfiguration der Motoren
    Motor1.setMaxSpeed(4000.0);
    Motor1.setAcceleration(2000.0);
    Motor2.setMaxSpeed(4000.0);
    Motor2.setAcceleration(2000.0);

    // Motoren zum MultiStepper hinzufügen
    Motoren.addStepper(Motor1);
    Motoren.addStepper(Motor2);
}

float MotorController::calculateSteps(float delta) {
    return round((delta * stepsPerRevolution) / (PI * diameter));
}

void MotorController::setPosition(int x, int y) {
    if (x <= maxXPosition && x >= minXPosition && y <= maxYPosition && y >= minYPosition) {
        int deltaX = x - currentXPosition;
        int deltaY = y - currentYPosition;

        Motor1.setCurrentPosition(0);
        Motor2.setCurrentPosition(0);

        float m1Steps = calculateSteps(deltaX + deltaY);
        float m2Steps = calculateSteps(deltaX - deltaY);

        long positions[2] = {static_cast<int>(m1Steps), static_cast<int>(m2Steps)};
        Motoren.moveTo(positions);

        currentXPosition = x;
        currentYPosition = y;
    } else {
        Serial.println("Position außerhalb des erlaubten Bereichs!");
    }
}

void MotorController::moveToPosition(int x, int y) {
    setPosition(x, y);
    Motoren.runSpeedToPosition();
}

void MotorController::enableMotors() {
    digitalWrite(motor1EnablePin, LOW);
    digitalWrite(motor2EnablePin, LOW);
}

void MotorController::disableMotors() {
    digitalWrite(motor1EnablePin, HIGH);
    digitalWrite(motor2EnablePin, HIGH);
}

void MotorController::homeMotors() {
    // Bewegung zur Home-Position
    setPosition(maxXPosition, minYPosition);
    while (digitalRead(endstopYPin) != LOW) {
        Motoren.run();
    }

    setPosition(minXPosition, minYPosition);
    while (digitalRead(endstopXPin) != LOW) {
        Motoren.run();
    }

    currentXPosition = minXPosition;
    currentYPosition = minYPosition;
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);
}
