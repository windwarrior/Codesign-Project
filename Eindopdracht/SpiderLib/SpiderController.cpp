#include "Arduino.h"
#include "SpiderController.h"

SpiderController::SpiderController(int leftpin, int middlepin, int rightpin){
    _left.attach(leftpin);
    _right.attach(rightpin);
    _middle.attach(middlepin);
    _left.calibrate(102, 17);
    _middle.calibrate(95, 17);
    _right.calibrate(78, 17);     
}

//Public
void SpiderController::turnLeft()
{
    turn(TURN__left);
}

void SpiderController::turnRight()
{
    turn(TURN__right);
}

void SpiderController::forward()
{
  sweepFromTo(_middle,  _middle.getMin());
  delay(100); 
  int rangeLeft = _left.getMax() - _left.getPosition();
  int rangeRight = _right.getMax() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, rangeRight);
  delay(100);
  
  sweepFromTo(_middle, _middle.getMax());
  delay(100);
  rangeLeft = _left.getMin() - _left.getPosition();
  rangeRight = _right.getMin() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, rangeRight);
  delay(100);
}

void SpiderController::back()
{

}

void SpiderController::reset()
{
   _left.write(leftMin + (range/2));
   _middle.write(midMin + ((midMax-midMin)/2));
   _right.write(rightMin + (range/2));
   delay(3000);
}


//Private
void SpiderController::sweepTwo(Motor mot1, int range1, Motor mot2, int range2)
{
  int to1 = mot1.getPosition() + range1;
  int to2 = mot2.getPosition() + range2;
  int curr1 = mot1.getPosition();
  int curr2 = mot2.getPosition();
  while(to1 != curr1 || to2 != curr2){
     //first servo
     if(range1 < 0){
        if(curr1 > to1 && curr1 >= 0 && curr1 <= 180){
           mot1.write(curr1);
           curr1--;
        }
     } else {
         if(curr1 < to1 && curr1 >= 0 && curr1 <= 180){
           mot1.write(curr1);
           curr1++;
         }
     } 
     
     //second servo
     if(range2 < 0){
        if(curr2 > to2 && curr2 >= 0 && curr2 <= 180){
           mot2.write(curr2);
           curr2--;
        }
     } else {
         if(curr2 < to2 && curr2 >= 0 && curr2 <= 180){
           mot2.write(curr2);
           curr2++;
         }
     }   
     delay(2);  
  }
}


void SpiderController::sweepFromTo(Motor mot, int to)
{
  if(from > to){
    for(int i = from; i > to; i--){
      mot.write(i);
      delay(2);
    }
    mot.write(to); //TODO waarom dit eigenlijk?
  }else{
    for(int i = from; i < to; i++){
      mot.write(i);
      delay(2);
    }
    mot.write(to); //TODO waarom dit eigenlijk?
  }
  
}

void SpiderController::turn(int side){
  //First tilt the robot to a side
  if(side == TURNLEFT){
    sweepFromTo(_middle,  _middle.getMin());
    posMiddle = midMin;
  } else if(side == TURNRIGHT){
    sweepFromTo(_middle,  _middle.getMax());
    posMiddle = midMax;
  }

  //Then move its legs
  delay(500);
  int rangeLeft = _left.getMin() - _left.getPosition();
  int rangeRight = _right.getMax() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, rangeRight);
  delay(500);
  
  //Tilt it to its other side
  if(side == TURNLEFT){
    sweepFromTo(_middle,  _middle.getMax());
    posMiddle = midMax;
  } else if(side == TURNRIGHT){
    sweepFromTo(_middle,  _middle.getMin());
    posMiddle = midMin;
  }

  //And move its legs to the other side
  delay(500);
  rangeLeft = _left.getMax() - _left.getPosition();
  rangeRight = _right.getMin() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, posRight, rangeRight);
  delay(500);
}
