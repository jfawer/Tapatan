#include "MotorController.h"

MotorController::MotorController(int m1Step, int m1Dir, int m1Enable, int m2Step, int m2Dir, int m2Enable, int esX, int esY, int em, int emPolarity)
    : motor1StepPin(m1Step), motor1DirPin(m1Dir), motor1EnablePin(m1Enable),
      motor2StepPin(m2Step), motor2DirPin(m2Dir), motor2EnablePin(m2Enable),
      endstopXPin(esX), endstopYPin(esY), electromagnetPin(em), electromagnetPolarityPin(emPolarity),
      Motor1(AccelStepper::DRIVER, m1Step, m1Dir), Motor2(AccelStepper::DRIVER, m2Step, m2Dir), Motoren(),
      currentXPosition(0), currentYPosition(0) {
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
    // Überprüfen, ob die Zielposition innerhalb der Grenzen liegt
    if (x > config.maxXPosition || x < config.minXPosition || y > config.maxYPosition || y < config.minYPosition) {
        Serial.println("Position ausserhalb des erlaubten Bereichs!");
        return; // Verlasse die Funktion
    }

    // Berechnung der Zielpositionen der Motoren (basierend auf H-Bot-Logik)
    long motor1Target = calculateSteps(x + y);
    long motor2Target = calculateSteps(x - y);

    // Zielpositionen der Motoren setzen
    Motor1.moveTo(motor1Target);
    Motor2.moveTo(motor2Target);

    Serial.print("Zielposition gesetzt: X = ");
    Serial.print(x);
    Serial.print(", Y = ");
    Serial.println(y);
}

/*
void MotorController::setPosition(int x, int y) {
    // Überprüfen, ob die Zielposition innerhalb der Grenzen liegt
    if (x > config.maxXPosition || x < config.minXPosition || 
        y > config.maxYPosition || y < config.minYPosition) {
        Serial.println("Position ausserhalb des erlaubten Bereichs!");
        return; // Verlasse die Funktion
    }

    // Berechnung der Differenzen zur aktuellen Position
    int deltaX = x - currentXPosition;
    int deltaY = y - currentYPosition;

    // Zielpositionen der Motoren berechnen (basierend auf H-Bot-Logik)
    long motor1Target = calculateSteps(deltaX + deltaY);
    long motor2Target = calculateSteps(deltaX - deltaY);

    // Zielpositionen der Motoren setzen
    Motor1.moveTo(Motor1.currentPosition() + motor1Target);
    Motor2.moveTo(Motor2.currentPosition() + motor2Target);

    // Aktualisieren der aktuellen Positionen
    currentXPosition = x;
    currentYPosition = y;

    Serial.print("Zielposition gesetzt: X = ");
    Serial.print(currentXPosition);
    Serial.print(", Y = ");
    Serial.println(currentYPosition);
}
*/

/*
void MotorController::setPosition(int x, int y) {
    if (x <= config.maxXPosition && x >= config.minXPosition && y <= config.maxYPosition && y >= config.minYPosition) {
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
        Serial.println("Position ausserhalb des erlaubten Bereichs!");
    }
}

*/

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

    // **Homing der Y-Achse**
    // Zielposition auf maximale X-Position und minimale Y-Position setzen
    long positions[2];
    positions[0] = config.maxXPosition; // Maximale X-Position
    positions[1] = config.minYPosition; // Minimale Y-Position

    // Bewegung starten und warten, bis der Y-Endschalter ausgelöst wird
    Motoren.moveTo(positions);
    while (digitalRead(endstopYPin) != LOW) {
        Motoren.run();
        Serial.println("Homing Y-Axis...");
    }

    // Zielposition der Y-Achse auf 0 setzen
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);

    // **Homing der X-Achse**
    // Zielposition auf minimale X-Position und aktuelle Y-Position setzen
    positions[0] = config.minXPosition; // Minimale X-Position
    positions[1] = 0; // Aktuelle Y-Position (bereits auf 0 gesetzt)

    // Bewegung starten und warten, bis der X-Endschalter ausgelöst wird
    Motoren.moveTo(positions);
    while (digitalRead(endstopXPin) != LOW) {
        Motoren.run();
        Serial.println("Homing X-Axis...");
    }

    // Zielposition der X-Achse auf 0 setzen
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);

    // Motoren deaktivieren
    disableMotors();

    Serial.println("Homing abgeschlossen! System ist nun in der Home-Position.");
}

/*
void MotorController::homeMotors() {
    enableMotors();

    // **Homing der Y-Achse**
    // Zielposition auf maximale X-Position und minimale Y-Position setzen
    
    // Bewegung starten und warten, bis der Y-Endschalter ausgelöst wird
    while (digitalRead(endstopYPin) != LOW) {
        Motor1.run();
        Motor2.run();
        Serial.println("Homing Y-Axis...");
    }
    
    // Zielposition der Y-Achse auf 0 setzen
    currentYPosition = config.minYPosition;

    // **Homing der X-Achse**
    // Zielposition auf minimale X-Position und aktuelle Y-Position setzen
    setPosition(config.minXPosition, currentYPosition);
    
    // Bewegung starten und warten, bis der X-Endschalter ausgelöst wird
    while (digitalRead(endstopXPin) != LOW) {
        Motor1.run();
        Motor2.run();
        Serial.println("Homing X-Axis...");
    }
    
    // Zielposition der X-Achse auf 0 setzen
    currentXPosition = config.minXPosition;

    // Motorpositionen zurücksetzen
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);

    // Motoren deaktivieren
    disableMotors();

    Serial.println("Homing abgeschlossen! System ist nun in der Home-Position.");
}
*/

/*
void MotorController::homeMotors() {
    
    // Bewegung zur Home-Position
    setPosition(config.maxXPosition, config.minYPosition);
    while (digitalRead(endstopYPin) != LOW) {
        Motoren.run();
    }

    setPosition(config.minXPosition, config.minYPosition);
    while (digitalRead(endstopXPin) != LOW) {
        Motoren.run();
    }

    currentXPosition = config.minXPosition;
    currentYPosition = config.minYPosition;
    Motor1.setCurrentPosition(0);
    Motor2.setCurrentPosition(0);
}
*/

// --------------------------------------------------------------------------------
// Funktion für die Bewegung des Motors je nach Spielzug (Platzierung, Verschieben, Aufräumen)
// --------------------------------------------------------------------------------

void MotorController::moveStone(Move move) {
    // Motor aktivieren
    enableMotors();

    // Konstanten für die Bewegung
    int startdelay = 2000;
    int delayTime = 500;

    // Überprüfen, ob sich die Startposition in einer Garage befindet
    if (isInGarage(move.startX, move.startY, config.computerGaragePosition) || isInGarage(move.startX, move.startY, config.playerGaragePosition)) {
        // Bestimmen der Fahrbahnen für die Bewegung
        int lane = determineShortestLane(move.startY, config.verticalLanePositions);               // Vertikale Fahrbahn
        int horizontalLane = determineShortestLane(move.targetX, config.horizontalLanePositions);  // Horizontale Fahrbahn
        
        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);
        delay(startdelay);
        moveToPosition(move.startX, config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[horizontalLane], config.verticalLanePositions[lane]);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[horizontalLane], move.targetY);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        return;
    }

    // Überprüfen, ob sich die Start- und Zielposition auf der gleichen Fahrbahn befinden
    if (isSameLane(move.startX, move.targetX, config.horizontalLanePositions)) {
        // Bestimmen der gemeinsamen Fahrbahn
        int lane = determineShortestLane(move.startX, config.horizontalLanePositions);              // Gemeinsame Fahrbahn

        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);
        delay(startdelay);
        moveToPosition(config.horizontalLanePositions[lane], move.startY);
        delay(delayTime);
        moveToPosition(config.horizontalLanePositions[lane], move.targetY);
        delay(delayTime);
        moveToPosition(move.targetX, move.targetY);
        return;

    } else {
        // Bestimmen der kürzesten Fahrbahn für Start- und Zielposition
        int startLane = determineShortestLane(move.startX, config.horizontalLanePositions);          // Startfahrbahn (Horizontale Fahrbahn)
        int targetLane = determineShortestLane(move.targetX, config.horizontalLanePositions);        // Zielfahrbahn  (Horizontale Fahrbahn)
        int verticalLane = determineShortestLane(move.targetY, config.verticalLanePositions);        // Vertikale Fahrbahn

        // Bewegung des Motors
        moveToPosition(move.startX, move.startY);
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
        return;
    }
    // Motor deaktivieren
    disableMotors();
}

// --------------------------------------------------------------------------------
// Funktionen für den Elektromagneten
// --------------------------------------------------------------------------------

void MotorController::electromagnetControl(bool state, bool polarity) {
    digitalWrite(electromagnetPolarityPin, polarity ? HIGH : LOW);
    digitalWrite(electromagnetPin, state ? HIGH : LOW);
}

// --------------------------------------------------------------------------------
// Setter für die gesamte Konfiguration
// --------------------------------------------------------------------------------

void MotorController::setConfig(const MotorConfig& newConfig) {
    config = newConfig;
}