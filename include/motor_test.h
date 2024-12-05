#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include <AccelStepper.h>
#include <MultiStepper.h>

// Struktur für einen Zug (Move)
struct Move {
    int x;
    int y;
    // Weitere Parameter wie Richtung oder Spielstein können hier hinzugefügt werden
};

class MotorTest {
public:
    void home();
    void moveToPosition(int x, int y);
    void enable();
    void disable();
    void moveStone(Move move);
    void setPosition(int x, int y);

private:
    MultiStepper Motoren;
    AccelStepper Motor1;
    AccelStepper Motor2;
    int motor1EnablePin;
    int motor2EnablePin;
    int currentXPosition;
    int currentYPosition;
    int maxXPosition;
    int maxYPosition;
    int minXPosition;
    int minYPosition;
    int endstopXPin;
    int endstopYPin;
    
    float calculateSteps(float delta);
    int findGaragestate(int garagestate[4], int value);
    void overlayBoard(int Board[3][3], int BoardMemory[3][3], int BoardOverlay[3][3]);
    Move determineMoveToPlace(int Board[3][3], int BoardMemory[3][3], int computerGarage[4], int computerGaragePosition[5][2], int BoardPosition[3][3][2]);
};

#endif // MOTOR_TEST_H