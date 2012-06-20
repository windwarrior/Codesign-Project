#include <Servo.h>

//OVERAL RANGE VAN 40 HOUDEN!
#define leftMax 120
#define leftMin 75

#define midMax 125
#define midMin 65

#define rightMax 105
#define rightMin 60

//LEFT  tussen 80 en 120
//RIGHT tussen 60 en 100
//MIDDLE tussen 65 en 125

Servo left;
Servo middle;
Servo right;

int range = 45;
int tmpRange = range;

int rangeLeft = range;
int rangeRight = range;

int posLeft = leftMin + (range/2);
int posMiddle = midMin + (range/2);
int posRight = rightMin + (range/2);

void setup()
{
  left.attach(9);
  middle.attach(10);
  right.attach(11);
  reset();
  delay(3000);
}

void loop()
{
 forward();
 //left.write(120);
}

void forward()
{
  sweepFromTo(middle, posMiddle, midMin);
  posMiddle = midMin;
  delay(500);
  //tmpRange = leftMax - posLeft;
  //sweepTwo(left, posLeft, right, posRight, tmpRange);
  //posLeft = posLeft + tmpRange;
  //posRight = posRight + tmpRange;  
  rangeLeft = leftMax - posLeft;
  rangeRight = rightMax - posRight;
  sweepTwoNew(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
  
  sweepFromTo(middle, posMiddle, midMax);
  posMiddle = midMax;  
  delay(500);
  //tmpRange = leftMin - posLeft;
  //sweepTwo(left, posLeft, right, posRight, tmpRange);
  //posLeft = posLeft + tmpRange;
  //posRight = posRight + tmpRange;
  rangeLeft = leftMin - posLeft;
  rangeRight = rightMin - posRight;
  sweepTwoNew(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
}

void turnLeft()
{
  sweepFromTo(middle, posMiddle, midMin);
  posMiddle = midMin;
  delay(500);
  rangeLeft = leftMin - posLeft;
  rangeRight = rightMax - posRight;
  sweepTwoNew(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
  
  sweepFromTo(middle, posMiddle, midMax);
  posMiddle = midMin;
  delay(500);
  rangeLeft = leftMax - posLeft;
  rangeRight = rightMin - posRight;
  sweepTwoNew(left, posLeft, rangeLeft, right, posRight, rangeRight);
  posLeft = posLeft + rangeLeft;
  posRight = posRight + rangeRight;
  delay(500);
}

void sweepFromTo(Servo serv, int from, int to){
  int start = millis();
  int duration = 500;
  if(from > to){
    for(int i = from; i > to; i--){
      serv.write(i);
      delay(5);
    }
  }else{
    for(int i = from; i < to; i++){
      serv.write(i);
      delay(5);
    }
  }
}

void sweepTwoNew(Servo serv1, int from1, int range1, Servo serv2, int from2, int range2){
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
     delay(5);  
  }
}

void sweepTwo(Servo serv1, int from1, Servo serv2, int from2, int range)
{
    int to1 = from1 + range;
    int to2 = from2 + range;
    int curr1 = from1;
    int curr2 = from2;
    if(range < 0){
      for(int i = 0; i > range; i--){
         if(curr1 > to1 && curr1 >= 0 && curr1 <= 180){
           serv1.write(curr1);
           curr1--;
         }
         if(curr2 > to2 && curr2 >= 0 && curr2 <= 180){
           serv2.write(curr2);
           curr2--;
         }
       }
    } else {
      for(int i = 0; i < range; i++){
         if(curr1 < to1 && curr1 >= 0 && curr1 <= 180){
           serv1.write(curr1);
           curr1++;
         }
         if(curr2 < to2 && curr2 >= 0 && curr2 <= 180){
           serv2.write(curr2);
           curr2++;
         }
       }
    }
}

void reset()
{
   left.write(leftMin + (range/2));
   middle.write(midMin + (range/2));
   right.write(rightMin + (range/2));
   delay(3000);
}
