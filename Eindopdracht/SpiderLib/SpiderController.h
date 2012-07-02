#ifndef SpiderController
#define SpiderController
#include "Arduino.h"
#include <Motor.h>

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
        void calibrate(Motor mot, int center, int range);     
    private:
        void sweepTwo(Motor mot1, int range1, Motor mot2, int range2);
        void sweepFromTo(Motor mot, int from, int to);
        void turn(int side);
        Motor _left;
        Motor _center;
        Motor _right;   
};
#endif
