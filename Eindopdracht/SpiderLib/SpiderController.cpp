#include "Arduino.h"
#include "SpiderController.h"
#include "Servo.h"

SpiderController::SpiderController(int leftpin, int middlepin, int rightpin){
    left.attach(leftpin);
    right.attach(rightpin);
    middle.attach(middlepin);

    calibrate(LEFT_MOTOR, 102, 17);
    calibrate(RIGHT_MOTOR, 78, 17);
    calibrate(CENTER_MOTOR, 95, 17);    
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
  sweepFromTo(middle, middlePos, middleMin);
  middlePos = midMin;
  delay(100); 
  rangeLeft = leftMax - leftPos;
  rangeRight = rightMax - rightPos;
  sweepTwo(LEFT_MOTOR, rangeLeft, RIGHT_MOTOR, rangeRight);
  delay(100);
  
  sweepFromTo(MIDDLE_MOTOR, midMax);
  delay(100);
  rangeLeft = leftMin - leftPos;
  rangeRight = rightMin - rightPos;
  sweepTwo(LEFT_MOTOR, rangeLeft, RIGHT_MOTOR, rangeRight);
  delay(100);
}

void SpiderController::back()
{

}

void SpiderController::reset()
{
   left.write(leftMin + (range/2));
   middle.write(midMin + ((midMax-midMin)/2));
   right.write(rightMin + (range/2));
   delay(3000);
}

void SpiderController::calibrate(Motor mot, int center, int range){
    if(mot == LEFT_MOTOR){
        leftMin = center - range;
        leftCenter = center;
        leftMax = center + range;
    }else if(mot == RIGHT_MOTOR){
        rightMin = center - range;
        rightCenter = center;
        rightMax = center + range;
    }else if(mot == MIDDLE_MOTOR{
        middleMin = center - range;
        middleCenter = center;
        middleMax = center + range;

    }
}

//Private
void SpiderController::sweepTwo(Motor mot1, int range1, Motor mot2, int from2)
{
  Servo serv1 =  mot == LEFT_MOTOR ? left : mot == RIGHT_MOTOR ? right : middle;
  Servo serv2 =  mot == LEFT_MOTOR ? left : mot == RIGHT_MOTOR ? right : middle;
  int from1 =  mot == LEFT_MOTOR ? leftPos : mot == RIGHT_MOTOR ? rightPos : middlePos;
  int from2 =  mot == LEFT_MOTOR ? leftPos : mot == RIGHT_MOTOR ? rightPos : middlePos;
  int to1 = from1 + range1;
  int to2 = from2 + range2;
  int curr1 = from1;
  int curr2 = from2;
  while(to1 != curr1 || to2 != curr2){
     //first servo
     if(range1 < 0){
        if(curr1 > to1 && curr1 >= 0 && curr1 <= 180){
           serv1.write(curr1);
           curr1--;
        }
     } else {
         if(curr1 < to1 && curr1 >= 0 && curr1 <= 180){
           serv1.write(curr1);
           curr1++;
         }
     } 
     
     //second servo
     if(range2 < 0){
        if(curr2 > to2 && curr2 >= 0 && curr2 <= 180){
           serv2.write(curr2);
           curr2--;
        }
     } else {
         if(curr2 < to2 && curr2 >= 0 && curr2 <= 180){
           serv2.write(curr2);
           curr2++;
         }
     }   
     delay(2);  
  }
}


void SpiderController::sweepFromTo(Motor mot, int to)
{
  Servo serv =  mot == LEFT_MOTOR ? left : mot == RIGHT_MOTOR ? right : middle;
  int from =  mot == LEFT_MOTOR ? leftPos : mot == RIGHT_MOTOR ? rightPos : middlePos;
  if(from > to){
    for(int i = from; i > to; i--){
      serv.write(i);
      delay(2);
    }
    serv.write(to);
  }else{
    for(int i = from; i < to; i++){
      serv.write(i);
      delay(2);
    }
    serv.write(to);
  }
  
}

void SpiderController::turn(int side){
  if(side == TURN_LEFT){
    sweepFromTo(middle, posMiddle, midMin);
    posMiddle = midMin;
  } else if(side == TURN_RIGHT){
    sweepFromTo(middle, posMiddle, midMax);
    posMiddle = midMax;
  }
  delay(500);
  rangeLeft = leftMin - posLeft;
  rangeRight = rightMax - posRight;
  sweepTwo(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
  
  if(side == TURN_LEFT){
    sweepFromTo(middle, posMiddle, midMax);
    posMiddle = midMax;
  } else if(side == TURN_RIGHT){
    sweepFromTo(middle, posMiddle, midMin);
    posMiddle = midMin;
  }
  delay(500);
  rangeLeft = leftMax - posLeft;
  rangeRight = rightMin - posRight;
  sweepTwo(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
}
