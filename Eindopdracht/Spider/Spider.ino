#include <SPI.h>
#include <Servo.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
//#include "Spider.h"

//RADIO
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xd250dbcf39LL, 0x4aac2e23feLL };
const int channel = 67;
const int retries = 5;
boolean ready = false;

//SPIDERWALK
//LEFT  tussen 80 en 120
//RIGHT tussen 60 en 100
//MIDDLE tussen 65 en 125
#define leftMax 120
#define leftMin 85

#define midMax 125
#define midMin 65

#define rightMax 95
#define rightMin 60

#define TURNLEFT 0
#define TURNRIGHT 1

Servo left;
Servo middle;
Servo right;

int range = 35;

int rangeLeft = range;
int rangeRight = range;

int posLeft = leftMin + (range/2);
int posMiddle = midMin + ((midMax-midMin)/2);
int posRight = rightMin + (range/2);

int currentDirection = 0;

void setup(){
  printf_begin();
  pinMode(13, OUTPUT);
  left.attach(3);
  middle.attach(5);
  right.attach(6);
  reset();
  Serial.begin(57600);
  setupRadio();
  delay(3000);
}

void loop(){
  //handle remote control
  boolean ready = false;
  boolean timeout = false;
  int time = millis();
  
  while(!ready && !timeout){
    if(radio.available()){
      ready = true;
    }
    //if(millis() - time > 10){
       timeout = true; 
    //}
  }
  if(ready){ 
    char msg[32];
    boolean isRead = radio.read(&msg, 32);
  
    Serial.println("---------------------------");
    Serial.print("Message received: ");
    Serial.println(msg);  
  }
  if(timeout && !ready){
     Serial.println("timeout"); 
  }
  //handle compass
  //handle radio
  delay(500);
}

//RADIO FUNCTIONS
void setupRadio(){
  radio.begin();
  delay(20);
  radio.setChannel(channel);
  radio.setRetries(15,15);
  radio.setPayloadSize(32);

  radio.openWritingPipe(pipes[1]);
  
  //TODO: afvragen  of dit strict noodzakelijk is
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();

  radio.printDetails();
}

void receiveDirection(){
  
}

void sendHeading(){
  
}

//SPIDERWALK FUNCTIONS
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
