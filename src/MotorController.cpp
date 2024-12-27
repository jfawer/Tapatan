#include "MotorController.h"

MotorController::MotorController(int m1Step, int m1Dir, int m1Enable, int m2Step, int m2Dir, int m2Enable, int esX, int esY, int em, int emPolarity)
    : motor1StepPin(m1Step), motor1DirPin(m1Dir), motor1EnablePin(m1Enable),
      motor2StepPin(m2Step), motor2DirPin(m2Dir), motor2EnablePin(m2Enable),
      endstopXPin(esX), endstopYPin(esY), electromagnetPin(em), electromagnetPolarityPin(emPolarity),
      Motor1(AccelStepper::DRIVER, m1Step, m1Dir), Motor2(AccelStepper::DRIVER, m2Step, m2Dir), Motoren() {
}

// --------------------------------------------------------------------------------
// Initialisierungsfunktionen
// --------------------------------------------------------------------------------

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
    pinMode(electromagnetPin, OUTPUT);
    pinMode(electromagnetPolarityPin, OUTPUT);

    // Konfiguration der Motoren
    Motor1.setMaxSpeed(4000.0);
    Motor1.setAcceleration(2000.0);
    Motor2.setMaxSpeed(4000.0);
    Motor2.setAcceleration(2000.0);

    // Motoren zum MultiStepper hinzufügen
    Motoren.addStepper(Motor1);
    Motoren.addStepper(Motor2);
}

// --------------------------------------------------------------------------------
// Private Hilfsfunktionen
// --------------------------------------------------------------------------------

float MotorController::calculateSteps(float delta) {
    return round((delta * stepsPerRevolution) / (PI * diameter));
}

void MotorController::setPosition(int x, int y) {
    // Grenzen überprüfen
    if (x > config.maxXPosition || x < config.minXPosition || y > config.maxYPosition || y < config.minYPosition) {
        Serial.println("Position ausserhalb des erlaubten Bereichs!");
        return;
    }

    // Schritte berechnen
    long m1StepsToDo = calculateSteps(-x) + calculateSteps(y);
    long m2StepsToDo = calculateSteps(-x) - calculateSteps(y);

    // Setze die Schritte für die Motoren
    long positions[2] = {(int)m1StepsToDo, (int)m2StepsToDo};
    Motoren.moveTo(positions);
}

int MotorController::determineShortestLane(int col, const int axisLane[2]) const {
    int distance1 = abs(axisLane[0] - col);
    int distance2 = abs(axisLane[1] - col);

    if (distance1 < distance2) {
        return 0;
    } else if (distance1 == distance2) {
        return 1;
    } else {
        return 1;
    }
}

bool MotorController::isSameLane(int startCol, int targetCol, const int axisLane[2]) const {
    int startLane = determineShortestLane(startCol, axisLane);
    int targetLane = determineShortestLane(targetCol, axisLane);

    return startLane == targetLane;
}

bool MotorController::isInGarage(int row, int col, const int garagePosition[5][2]) const {
    for (int i = 0; i < 5; i++) {
        if (garagePosition[i][0] == row && garagePosition[i][1] == col) {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------------
// Motorsteuerungsfunktionen
// --------------------------------------------------------------------------------

void MotorController::moveToPosition(int x, int y) {
    // Motoren aktivieren
    enableMotors();

    // Setze die Zielposition
    setPosition(x, y);

    // Bewegung der Motoren
    Motoren.runSpeedToPosition();

    // Motoren deaktivieren
    disableMotors();
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
    enableMotors();
    int homingSpeed = 2000;

    // **Schritt 1: Y-Endschalter erreichen (diagonale Bewegung)**
    while (digitalRead(endstopYPin) != LOW) {
        Motor1.setSpeed(-homingSpeed); // Motor 1 gegen den Uhrzeigersinn
        Motor2.setSpeed(homingSpeed);  // Motor 2 im Uhrzeigersinn
        Motor1.run();
        Motor2.run();
    }

    // Zielposition der Y-Achse auf 0 setzen
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);

    // **Schritt 2: X-Endschalter erreichen (lineare Bewegung)**
    while (digitalRead(endstopXPin) != LOW) {
        Motor1.setSpeed(homingSpeed); // Beide Motoren gegen den Uhrzeigersinn
        Motor2.setSpeed(homingSpeed);
        Motor1.run();
        Motor2.run();
    }

    // Zielposition der X-Achse auf 0 setzen
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);

    // Motoren deaktivieren
    disableMotors();
}

// --------------------------------------------------------------------------------
// Funktion für die Bewegung des Motors je nach Spielzug (Platzierung, Verschieben, Aufräumen)
// --------------------------------------------------------------------------------

void MotorController::moveStone(Move move) {
    // Motor aktivieren
    enableMotors();

    // Konstanten für die Bewegung
    int startdelay = 4000;
    int delayTime = 500;

    // Überprüfen, ob sich die Startposition in einer Garage befindet
    if (isInGarage(move.startX, move.startY, config.computerGaragePosition) || isInGarage(move.startX, move.startY, config.playerGaragePosition)) {
        // Bestimmen der Fahrbahnen für die Bewegung
        int lane = determineShortestLane(move.startY, config.verticalLanePositions);                    // Vertikale Fahrbahn
        int horizontalLane = determineShortestLane(move.targetX, config.horizontalLanePositions);       // Horizontale Fahrbahn
        
        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);

        if (isInGarage(move.startX, move.startY, config.computerGaragePosition)) {
            turnElectromagnetOn();                                                                      // Elektromagnet einschalten
            setElectromagnetPolarityNegative();                                                         // Positive Polarität einstellen
        } else {
            turnElectromagnetOn();                                                                      // Elektromagnet einschalten
            setElectromagnetPolarityPositive();                                                         // Negative Polarität einstellen
        }

        delay(startdelay);
        moveToPosition(move.startX, config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[horizontalLane], config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[horizontalLane], move.targetY);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        delay(delayTime);
        turnElectromagnetOff();                                                                         // Elektromagnet ausschalten
        
        if (isInGarage(move.startX, move.startY, config.computerGaragePosition)) {
            setElectromagnetPolarityPositive();                                                         // Negative Polarität einstellen
        } else {
            setElectromagnetPolarityNegative();                                                         // Positive Polarität einstellen
        }

        // Beweg zur Garage zurück
        moveToPosition(config.horizontalLanePositions[horizontalLane], move.targetY);
        moveToPosition(config.horizontalLanePositions[horizontalLane], config.verticalLanePositions[lane]);
        moveToPosition(move.startX, config.verticalLanePositions[lane]);
        moveToPosition(move.startX, move.startY);        
        return;
    }

    // Überprüfen ob sich die Zielposition in einer Garage befindet
    if (isInGarage(move.targetX, move.targetY, config.computerGaragePosition) || isInGarage(move.targetX, move.targetY, config.playerGaragePosition)) {
        // Bestimmen der Fahrbahnen für die Bewegung
        int lane = determineShortestLane(move.targetY, config.verticalLanePositions);                   // Vertikale Fahrbahn
        int horizontalLane = determineShortestLane(move.startX, config.horizontalLanePositions);        // Horizontale Fahrbahn

        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);

        if (isInGarage(move.startX, move.startY, config.computerGaragePosition)) {
            turnElectromagnetOn();                                                                      // Elektromagnet einschalten
            setElectromagnetPolarityNegative();                                                         // Positive Polarität einstellen
        } else {
            turnElectromagnetOn();                                                                      // Elektromagnet einschalten
            setElectromagnetPolarityPositive();                                                         // Negative Polarität einstellen
        }

        delay(startdelay);
        moveToPosition(config.horizontalLanePositions[horizontalLane], move.startY);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[horizontalLane], config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(move.targetX, config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        delay(delayTime);
        turnElectromagnetOff();                                                                         // Elektromagnet ausschalten
        return;
    }

    // Überprüfen, ob sich die Start- und Zielposition auf der gleichen Fahrbahn befinden
    if (isSameLane(move.startX, move.targetX, config.horizontalLanePositions)) {
        // Bestimmen der gemeinsamen Fahrbahn
        int lane = determineShortestLane(move.startX, config.horizontalLanePositions);                  // Gemeinsame Fahrbahn

        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);
        turnElectromagnetOn();                                                                          // Elektromagnet einschalten
        setElectromagnetPolarityNegative();                                                             // Negative Polarität einstellen
        delay(startdelay);
        moveToPosition(config.horizontalLanePositions[lane], move.startY);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[lane], move.targetY);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        turnElectromagnetOff();                                                                         // Elektromagnet ausschalten
        return;

    } else {
        // Bestimmen der kürzesten Fahrbahn für Start- und Zielposition
        int startLane = determineShortestLane(move.startX, config.horizontalLanePositions);             // Startfahrbahn (Horizontale Fahrbahn)
        int targetLane = determineShortestLane(move.targetX, config.horizontalLanePositions);           // Zielfahrbahn  (Horizontale Fahrbahn)
        int verticalLane = determineShortestLane(move.targetY, config.verticalLanePositions);           // Vertikale Fahrbahn

        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);
        turnElectromagnetOn();                                                                          // Elektromagnet einschalten
        setElectromagnetPolarityNegative();                                                             // Negative Polarität einstellen
        delay(startdelay);
        moveToPosition(config.horizontalLanePositions[startLane], move.startY);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[startLane], config.verticalLanePositions[verticalLane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[targetLane], config.verticalLanePositions[verticalLane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[targetLane], move.targetY);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        turnElectromagnetOff();                                                                         // Elektromagnet ausschalten
        return;
    }
    // Motor deaktivieren
    disableMotors();
}

// --------------------------------------------------------------------------------
// Funktionen für den Elektromagneten
// --------------------------------------------------------------------------------

// Funktion zum Einschalten des Elektromagneten
void MotorController::turnElectromagnetOn() {
    digitalWrite(electromagnetPin, HIGH);                           // Elektromagnet einschalten
}

// Funktion zum Ausschalten des Elektromagneten
void MotorController::turnElectromagnetOff() {
    digitalWrite(electromagnetPin, LOW);                            // Elektromagnet ausschalten
}

// Funktion für die Positive Polarität des Elektromagneten
void MotorController::setElectromagnetPolarityPositive() {
    digitalWrite(electromagnetPolarityPin, HIGH);                   // Positive Polarität einstellen
}

// Funktion für die Negative Polarität des Elektromagneten
void MotorController::setElectromagnetPolarityNegative() {
    digitalWrite(electromagnetPolarityPin, LOW);                    // Negative Polarität einstellen
}

// --------------------------------------------------------------------------------
// Setter für die gesamte Konfiguration
// --------------------------------------------------------------------------------

void MotorController::setConfig(const MotorConfig& newConfig) {
    config = newConfig;
}