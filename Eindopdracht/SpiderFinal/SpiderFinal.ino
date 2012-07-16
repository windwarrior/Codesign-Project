#include <Wire.h>
#include <Motor.h>
#include <SpiderController.h>
#include "SpiderFinal.h"
#include <SPI.h>
#include <Servo.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Compass.h>

#define DEBUGLED 4
#define ROLEPIN 14
//RADIO
RF24 radio(9,10);

const uint64_t fromRemoteToSpider1 = 0x1761D0F64All;
const uint64_t fromSpider1ToRemote = 0x1761D0F64Bll;
const uint64_t fromSpider1ToSpider2 = 0x1761D0F64Cll;
const uint64_t fromSpider2ToSpider1 = 0x1761D0F64Dll;
const int channel = 88;
const int retries = 5;
bool isLeader = false;
bool hasReceived = false;
void check_radio(void); //TODO: geen idee, don't care

char lastMsg[32];
char tmp_msg[32];
//SPIDERWALK
//LEFT  tussen 80 en 120
//RIGHT tussen 60 en 100
//MIDDLE tussen 65 en 125
SpiderController control;
Compass compass;

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
  pinMode(DEBUGLED, OUTPUT);
  pinMode(ROLEPIN, INPUT);
  digitalWrite(ROLEPIN, HIGH);
  delay(200);
  isLeader = !digitalRead(ROLEPIN) ? true : false;
  Serial.begin(57600);
  control.begin(7,5,6);
  setupRadio();
  delay(3000);
  if(isLeader){   
    Serial.println("Red leader standing by!"); 
    delay(1000);
    Serial.println("Gold leader standing by!");
  } else {
     radio.stopListening(); 
  }
}

void loop(){
  //handle remote control
  if(hasReceived){
    Serial.println("JONGENS, IK HEB SHIT ONTVANGEN");
    hasReceived = false;
  }

  if(isLeader){
    //sendHandshakeToRemote();
    //receiveHeadingFromRemote();
    //sendHeadingToFollower();
  }else{
    //oke we moeten  dus interrupten en wachten op data
    interruptLeader();
    delay(200);
    //receiveHeadingFromLeader();
  }
  //control.back();
  //Serial.println(compass.getHeading());
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

  if(isLeader){
    //radio.openWritingPipe(fromSpider1ToRemote);
    //radio.openReadingPipe(1, fromRemoteToSpider1);
    radio.openWritingPipe(fromSpider1ToSpider2);
    radio.openReadingPipe(2, fromSpider2ToSpider1);
  } else {
    radio.openWritingPipe(fromSpider2ToSpider1);
    radio.openReadingPipe(1, fromSpider1ToSpider2);
  }

  radio.startListening();

  radio.printDetails();

  if(isLeader){
    attachInterrupt(0, check_radio, FALLING);
  }    
}

void sendHandshakeToRemote(){
  radio.stopListening();
  char message[32];
  message[31] = 0x00;
  message[1] = '1';
  bool isSend = false;
  int i = 0;
  while(!isSend && i < retries){
    bool isSend = radio.write(message, 32);
    if(isSend){
      Serial.println("Request sent");  
    }
    i++;
  }  
  radio.startListening();
  //delay(20);//TODO: KORTER MAKEN!
}

void receiveHeadingFromRemote(){
  long time = millis() + 50;//200ms timeout
  boolean timeout = false;
  boolean ready = false;
  digitalWrite(DEBUGLED, HIGH); //Kan het niet zo zijn dat deze heel snel weer getriggerd wordt?
  while(!ready && !timeout){
    if(radio.available()){
      ready = true;
    }
    if(millis() > time){
      timeout = true;
    }
  }
  digitalWrite(DEBUGLED, LOW); //Kan het niet zo zijn dat deze heel snel weer getriggerd wordt?
  
  boolean isRead = false;
  if(ready){ 
    char msg[32];
    
    while(!isRead){
      isRead = radio.read(&msg, 32);
      //delay(20);
    }
    
    currentDirection = getDirection(msg[0]);
  
    Serial.println("---------------------------");
    Serial.print("Message received: ");
    Serial.println(msg[0]);
   for(int i=0; i<32; i++){
     lastMsg[i] = msg[i];
   }
  }
  if(!ready && timeout){
     Serial.println("Timeout..."); 
  }
  
  if(isRead){
    radio.stopListening();
    //delay(20);
    moveSpider();
    radio.startListening();
    //delay(20);
  }
}

void check_radio(void){
  bool tx,fail,rx;
  radio.whatHappened(tx, fail, rx);

  if(rx){
    //We ontvangen dus shit, hooraay!
    //uint8_t tmpPipe = 2;
    //if(radio.available()){//&tmpPipe
    //    hasReceived = true;
    //}
    if(radio.read(&tmp_msg,32)){
       hasReceived = true; 
    }
  }

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
     control.back();
      Serial.println("achter");
   } else if(currentDirection == NONE) {
       Serial.println("niks"); 
   }
}

void interruptLeader(){
  //radio.stopListening();
  int i = 0;
  char message[32];
  message[31] = 0x00;
  message[1] = '1';
  bool isSend = false;
  while(!isSend && i < retries){
    //Serial.println("Starting interrupt write");
    isSend = radio.write(&message, 32);
    delay(20);
    //Serial.println("Ending interrupt write");
    if(isSend){
      Serial.println("Interrupt sent");  
    } else {
       Serial.println("Failed to sent interrupt"); 
    }
    i++;
  }  
  //radio.startListening();
} 

void receiveHeadingFromLeader(){
  boolean ready = false;
  long time = millis() + 100;
  while(!ready){
    if(radio.available()){
      ready = true;
    }
    if(millis() > time){
      Serial.println("Sending interrupt....");
      interruptLeader();
      time = millis() + 100;
    }
  }

  //read the message
  //think about what to do
  //run around in circles
  //think again
  //consider doing something
  //leave it be
}

void sendHeadingToFollower(){
  if(hasReceived){
    int i = 0;
    radio.openWritingPipe(fromSpider1ToSpider2);
    radio.stopListening();
    bool isSend = false;
    while(!isSend && i < retries){
      isSend = radio.write(lastMsg, 32);
      if(isSend){
        Serial.println("Sent heading to follower spider");  
      } else {
        Serial.println("Failed to sent heading to follower spider");
      }
      i++;
    }  
    radio.startListening();  
    radio.openWritingPipe(fromSpider1ToRemote);
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
