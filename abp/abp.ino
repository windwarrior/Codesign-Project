#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#define channel 67
#define HOP

#define SENDER 0
#define RECEIVER 0
RF24 radio(3,9);
void setup(void){
  Serial.begin(57600);
  radio.begin();
  delay(20);

  radio.setChannel(channel);
  radio.printDetails();

}

void loop(void){
#ifdef HOP
  radio.startListening();
  bool timeout = false;
  boolean ready = false;
  uint64_t sendTo =  0;
  while (!ready){
    if(radio.available(SENDER)){
      ready = true;
      sendTo = RECEIVER;
    }

    if(radio.available(RECEIVER)){
      ready = true;
      sendTo = SENDER;
    }  
  }
  char readstring[32];
  radio.read(&readstring, 32);
  radio.openWritingPipe(sendTo);
  radio.write(&readstring, 32);
#endif

}






