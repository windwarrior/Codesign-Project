#ifndef Morse_h
#define Morse_h
#include "Arduino.h"
#include <Servo.h>

class SpiderController{
    public:
        SpiderController(int leftpin, int middlepin, int rightpin);
        void turnLeft();
        void turnRight();
        void forward();
        void back();
        void reset();
    private:
        void sweepTwo(Servo serv1, int from1, int range1, Servo serv2, int from2, int range2);
        void sweepFromTo(Servo serv, int from, int to);
        void turn(int side);
        Servo left;
        Servo middle;
        Servo right;
#endif