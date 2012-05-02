#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
//#define SENDER

#define HELO 1
#define HACK 2


RF24 radio(3,9);

const int channel = 57;

const uint64_t pipes[2] = {0xd250dbcf39LL, 0x4aac2e23feLL};

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
  
  #ifdef SENDER
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
    startSendingHelo();
  #else
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    startListeningForHelo();
  #endif
}

void startSendingHelo(void){
    boolean recievedHACK = false;
    while(!recievedHACK){
      radio.stopListening();
      unsigned long time = millis();
      
    }
      
}
void startListeningForHelo(void){

}

void loop(void){
  
}
