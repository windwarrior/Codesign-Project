#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"

//Settings
#define channel 67
#define payloadSize 32
RF24 radio(3,9);

//Addresses
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
  Serial.println("----------------------------");
  char msg[32];
  String msgString = "hello";
  msgString.toCharArray(msg,32);
  msg[31] = (char) 0x00;
  bool isSend = radio.write(msg, 32);

  if(isSend){
    Serial.print("Sent package ");
    Serial.println(msg);
    radio.startListening();

    boolean ready = false;

    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && !timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    if(timeout){
      Serial.println("Radio timed out");
    }
    else{
      char result[32];
      radio.read(&result, 32);

      Serial.print("Got message: ");
      Serial.println(result);
    }

  }
  else{
    Serial.println("Failed to sent package");
  }
  radio.startListening();
  delay(100);

}






