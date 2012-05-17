#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"

//settings
#define channel 67
#define payloadSize 8
RF24 radio(3,9);

//addresses
uint64_t senderToHop = 0x1761D0F64Dll;
uint64_t hopToSender = 0x1761D0F64All;

//Some variables for the alternating bit protocol
boolean seq = 0;//huidig sequence nummer
boolean receivedAck = false;  
unsigned int globSeq = 0;

void setup(void){
   Serial.begin(57600);
   printf_begin();
   radio.begin();
   delay(20);
   
   radio.setChannel(channel);
   radio.setRetries(15,15);
   radio.setPayloadSize(payloadSize);
   
   radio.openReadingPipe(1,hopToSender);
   radio.openWritingPipe(senderToHop);
   
   Serial.println("I am SENDER");
   radio.printDetails();
   radio.startListening();
}

void loop(void){  
  //Stop listening, start sending the string
  radio.stopListening();
 
  char seqChar = seq ? '1' : '0';
  
  bool isSend = radio.write(&seqChar, sizeof(char));
  
  if(isSend){
    Serial.print("Sent package ");
    Serial.println(seqChar);
    radio.startListening();
   
    boolean ready = false;
  
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && !timeout )
      if (millis() - started_waiting_at > 1000 )
        timeout = true;
        
    if(timeout){
      Serial.println("Radio timed out");
    }else{
      char gotChar;
      radio.read(&gotChar, sizeof(char));
      
      Serial.print("Got message: ");
      Serial.println(gotChar);
      
      if(gotChar == seqChar){
        seq = !seq;
      }
    }
        
  }else{
    Serial.println("Failed to sent package");
  }
  
  radio.startListening();
  delay(100);
  
}
  
  
  
  
  
