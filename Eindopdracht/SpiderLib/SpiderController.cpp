#include "Arduino.h"
#include "SpiderController.h"
SpiderController::SpiderController(){
}
void SpiderController::begin(int leftpin, int middlepin, int rightpin){   
    _left.attach(leftpin);
    _right.attach(rightpin);
    _middle.attach(middlepin);
    _left.calibrate(102, 17);
    _middle.calibrate(95, 17);
    _right.calibrate(78, 17);    
     reset(); 
}

//Public
void SpiderController::turnLeft()
{
    turn(TURN_LEFT);
}

void SpiderController::turnRight()
{
    turn(TURN_RIGHT);
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
   _left.reset();
   _middle.reset();
   _right.reset();
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
     delay(4);  
  }
}


void SpiderController::sweepFromTo(Motor mot, int to)
{
  if(mot.getPosition() > to){
    for(int i = mot.getPosition(); i > to; i--){
      mot.write(i);
      delay(4);
    }
    mot.write(to); //TODO waarom dit eigenlijk?
  }else{
    for(int i = mot.getPosition(); i < to; i++){
      mot.write(i);
      delay(4);
    }
    mot.write(to); //TODO waarom dit eigenlijk?
  }
  
}

void SpiderController::turn(Turn side){
  //First tilt the robot to a side
  if(side == TURN_LEFT){
    sweepFromTo(_middle,  _middle.getMin());
  } else if(side == TURN_RIGHT){
    sweepFromTo(_middle,  _middle.getMax());
  }

  //Then move its legs
  delay(500);
  int rangeLeft = _left.getMin() - _left.getPosition();
  int rangeRight = _right.getMax() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, rangeRight);
  delay(500);
  
  //Tilt it to its other side
  if(side == TURN_LEFT){
    sweepFromTo(_middle,  _middle.getMax());
  } else if(side == TURN_RIGHT){
    sweepFromTo(_middle,  _middle.getMin());
  }

  //And move its legs to the other side
  delay(500);
  rangeLeft = _left.getMax() - _left.getPosition();
  rangeRight = _right.getMin() - _right.getPosition();
  sweepTwo(_left, rangeLeft, _right, rangeRight);
  delay(500);
}

void SpiderController::test(){
  //test left motor
  _left.write(150);
  delay(1000);
  _left.write(30);
  delay(1000);
  //test middle motor
    _middle.write(150);
  delay(1000);
  _middle.write(30);
  delay(1000);
  //test right motor
    _right.write(150);
  delay(1000);
  _right.write(30);
  delay(1000);
}

