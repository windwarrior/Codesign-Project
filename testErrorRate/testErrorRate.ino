#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
#define SENDER
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
  boolean receivedHACK = false;
  while(!receivedHACK){
    radio.stopListening();
    char sendstring[] = "Hallo";
    boolean sent = radio.write(&sendstring, 5);
    if(sent){
      printf("Sent helo package \n\r");
    }
    else{
      printf("Failed to sent package with helo \n\r"); 
    }
    radio.startListening();

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
      char receiveString;
      radio.read(&receiveString,sizeof(char));
      if(receiveString == 'A'){
        Serial.println("Handshake done!");
        radio.stopListening();
        char someChar = 'X';
        radio.write(&someChar, sizeof(char));
        receivedHACK = true;
      }
    }
    delay(200);
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
  #ifdef SENDER
  for(int i = 0; i < 10; i++){
    String jeMoedersString = "ping ";
    jeMoedersString.concat(i);
    jeMoedersString.concat(" ");
    jeMoedersString.concat(millis());
    char jeMoedersStringInChars[32];
    jeMoedersString.toCharArray(jeMoedersStringInChars, 32);
    Serial.println(jeMoedersString);
    
    radio.stopListening();
    boolean blab = radio.write(&jeMoedersStringInChars, 32);
    radio.startListening();
    Serial.println(blab);
    unsigned long currtime = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout ){
      if (millis() - currtime > timeoutTime ){
        timeout = true;
      }
    }
    if(timeout){
      printf("Did not recieve ACK\n\r");
    } 
    else {
      char receiveString[32];
      radio.read(&receiveString,32);
      Serial.println(receiveString);
    }
  
    delay(100);  
  }
  #else
  
  #endif
}

