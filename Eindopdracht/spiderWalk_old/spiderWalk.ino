#include <Servo.h>

Servo left;
Servo middle;
Servo right;

int posLeft = 80;
int posMiddle = 90;
int posRight = 60;

//LEFT  tussen 80 en 120
//RIGHT tussen 60 en 100
//MIDDLE tussen 65 en 125

void setup()
{
  left.attach(9);
  middle.attach(10);
  right.attach(11);
  reset();
}

void loop()
{
 forward();
 //reset();
}

void forward()
{
  sweepFromTo(middle, posMiddle, 65);
  posMiddle = 65;  
  delay(500);
  sweepTwo(left, posLeft, right, posRight, 40);
  posLeft = posLeft + 40;
  posRight = posRight + 40;
  delay(500);
  
//  sweepFromTo(left, posLeft, 100);
//  posLeft = 100;
//  sweepFromTo(right, posRight, 80);
//  posRight = 80; 
//  
//  sweepFromTo(left, posLeft, 120);
//  posLeft = 120;
//  sweepFromTo(right, posRight, 100);
//  posRight = 100; 

  
  sweepFromTo(middle, posMiddle, 125);
  posMiddle = 125;  
  delay(500);
  sweepTwo(left, posLeft, right, posRight, -40);
  posLeft = posLeft - 40;
  posRight = posRight - 40;
  delay(500);
  
  
//  sweepFromTo(left, posLeft, 100);
//  posLeft = 100;//80
//  sweepFromTo(right, posRight, 80);
//  posRight = 80;
//  
//  sweepFromTo(left, posLeft, 80);
//  posLeft = 80;
//  sweepFromTo(right, posRight, 60);
//  posRight = 60;
  
  //reset();
}

void sweepFromTo(Servo serv, int from, int to){
  if(from > to){
    for(int i = from; i > to; i--){
      serv.write(i);
    }
  }else{
    for(int i = from; i < to; i++){
      serv.write(i);
    }
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
         
         delay(10);
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
         delay(10);
       }
    }
}

void reset()
{
   left.write(80);
   middle.write(90);
   right.write(60);
   delay(3000);
}
