#include <SPI.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>

#define ROLEPIN 14

RF24 radio(9,10);

const uint64_t fromSpider1ToSpider2 = 0x1761D0F64Cll;
const uint64_t fromSpider2ToSpider1 = 0x1761D0F64Dll;

const int channel = 88;
const int retries = 5;

bool isLeader = false;
bool hasReceived = false;

char tmp_msg[32];

void check_radio(void);

void setup(){
  Serial.begin(57600);
  printf_begin();
  
  pinMode(ROLEPIN, INPUT);
  digitalWrite(ROLEPIN, HIGH);
  delay(200);
  
  isLeader = !digitalRead(ROLEPIN) ? true : false;
  
  if(isLeader){
    Serial.println("I am RECEIVER");  
  } else {
    Serial.println("I am SENDER");
  }
  
  setupRadio();
  delay(500);
}

void setupRadio(){
  radio.begin();
  delay(20);
  radio.setChannel(channel);
  radio.setRetries(15,15);
  radio.setPayloadSize(32);
  
  if(isLeader){
    radio.openWritingPipe(fromSpider1ToSpider2);
    radio.openReadingPipe(1, fromSpider2ToSpider1);
    radio.startListening();
  } else {
    radio.openWritingPipe(fromSpider2ToSpider1);
    radio.openReadingPipe(1, fromSpider1ToSpider2);
  }
  
  if(isLeader){
    attachInterrupt(0, check_radio, FALLING);
  }    
  
  radio.printDetails();
}

void loop(){
  if(isLeader){
     //receiveInterrupt(); 
     if(hasReceived){
      Serial.print("Message received: ");
      Serial.println(tmp_msg); 
    }
    delay(500);
  } else {
    sendInterrupt();
  }
}

void sendInterrupt(){
  char message[] = "This is a message";
  bool isSend = false;
  while(!isSend){
     isSend = radio.write(&message, 32);
     if(isSend){
       Serial.println("Interrupt sent");  
     } else {
       Serial.println("Failed to sent interrupt"); 
     }
  }
}

void receiveInterrupt(){
  bool ready = false;
  while(!ready){
     if(radio.available()){
       ready = true;
     } 
  }
  
  char msg[32];
  bool isRead = false;
  while(!isRead){
    isRead = radio.read(&msg, 32);
  }
  
  Serial.print("Message received: ");
  Serial.println(msg);
}

void check_radio(void){
  bool tx,fail,rx;
  radio.whatHappened(tx, fail, rx);

  if(rx && isLeader){
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
