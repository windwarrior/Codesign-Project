#include <Motor.h>
#include <SpiderController.h>
#include "SpiderFinal.h"
#include <SPI.h>
#include <Servo.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>



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
SpiderController control;

//#define leftMax 120
//#define leftMin 85
//
//#define midMax 125
//#define midMin 65
//
//#define rightMax 95
//#define rightMin 60

int currentDirection = NONE;

void setup(){
  printf_begin();
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(57600);
  control.begin(3,5,6);
  setupRadio();
  delay(3000);
}

void loop(){
  //handle remote control
  receiveDirection();
  //control.forward();
  //handle compass
  //handle radio
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
  boolean ready = false;
  boolean timeout = false;
  int time = millis();
  
  radio.stopListening();
  
  //REQUEST DIRECTION
  char message[32];
  message[31] = 0x00;
  message[1] = '1';
  bool isSend = false;
  int i = 0;
  while(!isSend && i < retries){
    bool isSend = radio.write(message, 32);
    if(isSend){
      //Serial.println(i);  
    }  
    i++;
  }
  
  radio.startListening();
  
  //READ DIRECTION
  while(!ready){//kan if worden =P
    digitalWrite(4, HIGH);
    if(radio.available()){
      ready = true;
    }
  }
  digitalWrite(4, LOW);
  
  if(ready){ 
    char msg[32];
    boolean isRead = radio.read(&msg, 32);
    
    currentDirection = getDirection(msg[0]);//zoiets?
  
    Serial.println("---------------------------");
    Serial.print("Message received: ");
    Serial.println(msg[0]);  
  }
  if(timeout && !ready){
     Serial.println("Going to other stuff..."); 
  }
  Serial.println("Nothing happened");
  
  moveSpider();
  delay(20);//TODO: KORTER MAKEN!
}

void sendHeading(){
  
}

//SPIDERWALK FUNCTIONS
void moveSpider(){
   if(currentDirection == FORWARD){
     control.forward();
     Serial.println("voor");
   } else if(currentDirection == LEFT){
      control.turnLeft();
       Serial.println("links");
   } else if(currentDirection == RIGHT){
      control.turnRight();
       Serial.println("rechts");
   } else if(currentDirection == BACK){
     //wololo achteruit
      Serial.println("achter");
   } else if(currentDirection == NONE) {
       Serial.println("niks"); 
   }
}

//DIRECTION
Direction getDirection(char val){
  if(val == '0'){
    return FORWARD;
  } else if(val == '1'){
    return RIGHT;
  } else if(val == '2'){
    return BACK;
  } else if(val == '3'){
    return LEFT;
  } else {
    return NONE;
  }
}
