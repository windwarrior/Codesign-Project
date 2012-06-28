#ifndef Morse_h
#define Morse_h
#include "Arduino.h"
#include "Servo.h"
enum Motor{
    LEFT_MOTOR,
    RIGHT_MOTOR,
    CENTER_MOTOR
};

enum Turn{
    TURN_LEFT,
    TURN_RIGHT,
};

class SpiderController{
    public:
        SpiderController(int leftpin, int middlepin, int rightpin);
        void turnLeft();
        void turnRight();
        void forward();
        void back();
        void reset();
        void calibrate(Motor motor, int center, int range);
    private:
        void sweepTwo(Servo serv1, int from1, int range1, Servo serv2, int from2, int range2);
        void sweepFromTo(Servo serv, int from, int to);
        void turn(int side);

        Servo left;
        int leftMin;
        int leftCenter;
        int leftMax;
        int leftPos;

        Servo middle;
        int middleMin;
        int middleCenter;
        int middleMax;
        int middlePos;

        Servo right;
        int rightMin;
        int rightCenter;
        int rightMax;
        int rightPos;
};
#endif
