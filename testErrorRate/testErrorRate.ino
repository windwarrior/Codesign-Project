#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
//#define SENDER
#define timeoutTime 200
#define HELO 0
#define HACK 1
#define PING 2
#define ACK  3

RF24 radio(3,9);

#define channel 67

const uint64_t pipes[2] = {
  0xd250dbcf39LL, 0x4aac2e23feLL};

void setup(void){
  Serial.begin(57600);
  printf_begin();
#ifdef SENDER
  printf("Will send HELO\n\r");
#else
  printf("Will listen for HELO\n\r");
#endif

  radio.begin();

  delay(20);

  radio.setChannel(channel);

  radio.setRetries(15,15);

  radio.printDetails();
#ifdef SENDER
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  startSendingHelo();
#else
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  startListeningForHelo();
#endif
}

#ifdef SENDER
void startSendingHelo(void){
  boolean recievedHACK = false;
  while(!recievedHACK){
    radio.stopListening();
    char sendstring = 0;
    Serial.println(sendstring);
    Serial.println(HELO);
    boolean sent = radio.write(&sendstring, radio.getPayloadSize());
    if(sent){
      printf("Sent helo package \n\r");
    }
    else{
      printf("Failed to sent package with helo \n\r"); 
    }

    unsigned long currtime = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout ){
      if (millis() - currtime > timeoutTime ){
        timeout = true;
      }
    }
    if(timeout){
      printf("Did not recieve hack, resending ... \n\r");
    } 
    else {
      String receiveString;
      radio.read(&receiveString, radio.getPayloadSize());
      Serial.print("Got response: " );
      Serial.print(receiveString);
      Serial.println();
    }
    delay(20);
  }

}
#else
void startListeningForHelo(void){
  while(true){
    if(radio.available()){
      Serial.print("Payload received: ");
      bool done = false;
      char receiveChar[32];
      while (!done){
        done = radio.read(&receiveChar,  32);
        Serial.print("Response ");
        Serial.print(receiveChar[0]);
        //Serial.print(HELO);         
           
        if(receiveChar[0] == 'h'){
          Serial.print(" Receiving HELO ");
          delay(20);
          
          radio.stopListening();
          char result = 'A';
          boolean sent = radio.write(&result, radio.getPayloadSize());
          Serial.print(sent);
          radio.startListening();
        }
        
        Serial.println();
        
        delay(200);
      }
    }
  }
}
#endif

void loop(void){

}

