#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"
#define channel 67

RF24 radio(3,9);

void setup(void){
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  radio.setRetries(15,15);
  radio.setPayloadSize(8);
  radio.openWritingPipe(0x1761d0f64bll);
  radio.openReadingPipe(1, 0x1761d0f64all);
  radio.startListening();
  radio.printDetails();
}

void loop(void){
  while(!radio.available()){
    
  }
  
  char rec;
  radio.read(&rec, sizeof(char));
  Serial.println(rec);
}
