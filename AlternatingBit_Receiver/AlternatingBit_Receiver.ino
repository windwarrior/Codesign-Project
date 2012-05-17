#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"

//Settings
#define channel 67
#define payloadSize 8
RF24 radio(3,9);

//Adresses
uint64_t receiverToHop = 0x1761D0F64Cll;
uint64_t hopToReceiver = 0x1761D0F64Bll;

void setup(void){
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  delay(20);
  
  radio.setChannel(channel);
  radio.setRetries(15,15);
  radio.setPayloadSize(payloadSize);
  
  radio.openReadingPipe(1, hopToReceiver);
  radio.openWritingPipe(receiverToHop);
  Serial.println("I am RECEIVER");
  radio.printDetails();
  radio.startListening();
  
}

void loop(void){
  boolean ready = false;
  
  while(!ready){
    if(radio.available()){
      ready = true;
    }
  }

  char readChar;
  boolean isRead = radio.read(&readChar, sizeof(char));

  Serial.println("---------------------------");
  Serial.print("Message received: ");
  Serial.println(readChar);  
  radio.stopListening();
  if(isRead){
    boolean isSend = radio.write(&readChar, sizeof(char));
    if(isSend){
      Serial.println("Failed to send the char");
    }
  }else{
     Serial.println("Failed to read the buffer");
  }
  radio.startListening();
  Serial.println("----------------------------");
 
}
