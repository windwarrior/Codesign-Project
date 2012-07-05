#ifndef _SPIDER_H_
#define _SPIDER_H_
#include "Arduino.h"
#include "Motor.h"

enum Turn{
    TURN_LEFT,
    TURN_RIGHT,
};

class SpiderController{
    public:
        SpiderController();
        void turnLeft();
        void turnRight();
        void forward();
        void back();
	void begin(int leftpin, int middlepin, int rightpin);
        void reset();
        void calibrate(Motor mot, int center, int range);    
        void test();
    private:
        void sweepTwo(Motor mot1, int range1, Motor mot2, int range2);
        void sweepFromTo(Motor mot, int to);
        void turn(Turn side);
        Motor _left;
        Motor _middle;
        Motor _right;  
};
#endif
