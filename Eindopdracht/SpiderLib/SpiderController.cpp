#include "Arduino.h"
#include <Servo.h>

SpiderController::SpiderController(int leftpin, int middlepin, int rightpin){
    left.attach(leftpin);
    right.attach(rightpin);
    middle.attach(middlepin);
}

//Public
void SpiderController::turnLeft()
{

}

void SpiderController::turnRight()
{

}

void SpiderController::forward()
{

}

void SpiderController::back()
{

}

void SpiderController::reset()
{

}

//Private
void SpiderController::sweepTwo(Servo serv1, int from1, int range1, Servo serv2, int from2, int range2)
{

}


void SpiderController::sweepFromTo(Servo serv, int from, int to)
{

}

void SpiderController::turn(int side){

}