#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "printf.h"

//settings
#define channel 67
#define payloadSize 8
RF24 radio(3,9);

//reading addresses
uint64_t hopReceiverReadingPipeAddr  = 0x1761D0F64CLL;
uint64_t hopSenderReadingPipeAddr    = 0x1761D0F64DLL;

//reading pipes
uint8_t SENDER   = 1;
uint8_t RECEIVER = 2;

//writing addresses
uint64_t hopToSenderAddr   = 0x1761D0F64ALL;
uint64_t hopToReceiverAddr = 0x1761D0F64BLL;

void setup(void){
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  delay(20);
  
  radio.setChannel(channel);
  radio.setRetries(15,15);
  radio.setPayloadSize(payloadSize);
  
  radio.openReadingPipe(SENDER, hopSenderReadingPipeAddr);
  radio.openReadingPipe(RECEIVER, hopReceiverReadingPipeAddr);

  Serial.println("I am HOP");
  radio.printDetails();
  radio.startListening();
}

void loop(void){  
  boolean toSND = false;
  boolean ready = false;
  
  Serial.println("----------------------");
  
  while(!ready){
    uint8_t pipe;
    if(radio.available(&pipe))
      if(pipe == SENDER){
          ready = true;
          toSND = true; //moet false zijn
          Serial.println("Receiving from SENDER...");
      }else if(pipe == RECEIVER){
          ready = true;
          toSND = false;//moet true zijn
          Serial.println("Receiving from RECEIVER...");
      }      
  }
  
  //start reading
  char readString;
  boolean isRead = radio.read(&readString, sizeof(char));
  
  Serial.print("Bool read: ");
  Serial.println(isRead);
  
  Serial.print("Message received: ");
  Serial.println(readString);
  
  //start writing
  radio.stopListening();
  if(toSND == true){
    radio.openWritingPipe(hopToSenderAddr);
    Serial.println("Writing to SENDER...");
  } else {
    radio.openWritingPipe(hopToReceiverAddr);
    Serial.println("Writing to RECEIVER...");
  }
  delay(20);
  boolean isSend = radio.write(&readString, sizeof(char));
  Serial.print("Bool write: ");
  Serial.println(isSend);
  
  radio.startListening();
  
}

