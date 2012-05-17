#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"

//Settings
#define channel 67
#define payloadSize 32
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

  char msg[32];
  boolean isRead = radio.read(&msg, 32);

  Serial.println("---------------------------");
  Serial.print("Message received: ");
  Serial.println(msg);  
  radio.stopListening();
  if(isRead){
    char blab[] = "Haaiii";
    boolean isSend = radio.write(blab, 32);
    if(!isSend){
      Serial.println("Failed to send the char");
    }
  }else{
     Serial.println("Failed to read the buffer");
  }
  radio.startListening();
 
}
