#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"
#define channel 100

RF24 radio(3,9);

void setup(void){
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  delay(20);
  radio.setRetries(15,15);
  radio.setPayloadSize(8);  
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.openReadingPipe(1, 0xF0F0F0F0D2LL);  
  radio.printDetails();
}

void loop(void){
  char sendChar = 'a';
  boolean sent = radio.write(&sendChar, sizeof(char));
  if(sent){
    Serial.println("A great succes");
  }else{
    Serial.println("This is not working :(");
  }

  delay(100);  
}
