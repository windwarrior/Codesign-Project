#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
//#define SENDER
#define timeoutTime 200
enum messageTypes {
  HELO,
  HACK,
  PING,
  ACK
};

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

  radio.setRetries(0,0);

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
    unsigned long time = millis();
    String sendstring = "Hallo"; //HELO + " " + time;
    Serial.print(sendstring);
    Serial.println();
    boolean sent = radio.write(&sendstring, radio.getPayloadSize());
    if(sent){
      printf("Sent helo package at time %lu...\n\r", time);
    }
    else{
      printf("failed to sent package with helo \n\r"); 
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
    delay(200);
  }

}
#else
void startListeningForHelo(void){
  while(true){
    if(radio.available()){
      bool done = false;
      String receiveString;
      while (!done){
        done = radio.read(&receiveString,  radio.getPayloadSize());
        
       // Serial.print(receiveString);
       //Serial.println();
       // Serial.print("adfsfsdfsdfsdf");
       // Serial.println();
       printf("Bericht: %s", receiveString);
        
        delay(20);
      }
    }
  }
}
#endif

void loop(void){

}

