#include <Servo.h>

//OVERAL RANGE VAN 45 HOUDEN!
#define leftMax 120
#define leftMin 85

#define midMax 115
#define midMin 55

#define rightMax 85
#define rightMin 50

#define TURNLEFT 0
#define TURNRIGHT 1

//LEFT  tussen 80 en 120
//RIGHT tussen 60 en 100
//MIDDLE tussen 65 en 125

Servo left;
Servo middle;
Servo right;

int range = 35;

int rangeLeft = range;
int rangeRight = range;

int posLeft = leftMin + (range/2);
int posMiddle = midMin + ((midMax-midMin)/2);
int posRight = rightMin + (range/2);

void setup()
{
  left.attach(3);
  middle.attach(5);
  right.attach(6);
 //reset();
  delay(3000);
}

void loop()
{
 //forward();
// left.write(leftMax);
// right.write(rightMax);
// middle.write(midMax);
 
 left.write(leftMin);
 right.write(rightMin);
 middle.write(midMin);
}

void forward()
{
  sweepFromTo(middle, posMiddle, midMin);
  posMiddle = midMin;
  delay(100); 
  rangeLeft = leftMax - posLeft;
  rangeRight = rightMax - posRight;
  sweepTwo(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(100);
  
  sweepFromTo(middle, posMiddle, midMax);
  posMiddle = midMax;  
  delay(100);
  rangeLeft = leftMin - posLeft;
  rangeRight = rightMin - posRight;
  sweepTwo(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(100);
}

void turnLeft()
{
  turn(TURNLEFT);
}

void turnRight()
{
   turn(TURNRIGHT);
}

void turn(int side){
  if(side == TURNLEFT){
    sweepFromTo(middle, posMiddle, midMin);
    posMiddle = midMin;
  } else if(side == TURNRIGHT){
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
  
  if(side == TURNLEFT){
    sweepFromTo(middle, posMiddle, midMax);
    posMiddle = midMax;
  } else if(side == TURNRIGHT){
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

void sweepFromTo(Servo serv, int from, int to){
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

void sweepTwo(Servo serv1, int from1, int range1, Servo serv2, int from2, int range2){
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

void reset()
{
   left.write(leftMin + (range/2));
   middle.write(midMin + ((midMax-midMin)/2));
   right.write(rightMin + (range/2));
   delay(3000);
}
