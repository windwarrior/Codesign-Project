#ifndef _SPIDER_H_
#define _SPIDER_H_
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
        void sweepFromTo(Motor mot, int to);
        void turn(Turn side);
        Motor _left;
        Motor _middle;
        Motor _right;   
};
#endif
