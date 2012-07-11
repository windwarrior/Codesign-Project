#include <SPI.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "DummySpider.h"

RF24 radio(3,9);
const uint64_t pipes[2] = { 0xd250dbcf39LL, 0x4aac2e23feLL };
const int channel = 88;
const int retries = 5;

int currentDirection = NONE;


void setup(){
  printf_begin();
  pinMode(4, OUTPUT);
  Serial.begin(57600);
  setupRadio();
  delay(3000);
}

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

void loop(){
  //handle remote control
  sendHandshake();
  receiveHeading();
  //control.forward();
  //handle compass
  //handle radio
}

void sendHandshake(){
  Serial.println("=== Start Radio Message ===");
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
  delay(20);//TODO: KORTER MAKEN!
}

void receiveHeading(){
  int time = millis() + 100;//200ms timeout
  boolean timeout = false;
  boolean ready = false;
  digitalWrite(4, HIGH); //Kan het niet zo zijn dat deze heel snel weer getriggerd wordt?
  while(!ready && !timeout){
    if(radio.available()){
      ready = true;
    }
    if(millis() > time){
      timeout = true;
    }
  }
  digitalWrite(4, LOW); //Kan het niet zo zijn dat deze heel snel weer getriggerd wordt?
  
  boolean isRead = false;
  if(ready){ 
    char msg[32];
    isRead = radio.read(&msg, 32);
    
    currentDirection = getDirection(msg[0]);
  
    Serial.print("Message received: ");
    Serial.println(msg[0]);  
  }
  if(!ready && timeout){
     Serial.println("Timeout..."); 
  }
  
  if(isRead){
    Serial.println("Waiting for some time!");
    delay(1500);
  } else {
    Serial.println("No reading :(");
  }
  
  Serial.println("===  End Radio Message  ===");
  Serial.println("");
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

