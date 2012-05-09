#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
//#define SENDER
#define timeoutTime 200
#define HELO "helo"
#define HACK "hack"
#define PING "ping"
#define ACK  "ack"

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
    char sendstring[] = HELO;
    boolean sent = radio.write(&sendstring, 32);
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
      bool done = false;
      char receiveChar[32];
      while (!done){
        done = radio.read(&receiveChar,  32);
        if(receiveChar[0] == 'H'){
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
    char msg[32];
    generatePingMessage(msg, i);    
    Serial.println(generatePingMessage);
    radio.stopListening();
    boolean sent = radio.write(&msg, 32);    
    radio.startListening();

    unsigned long currtime = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout ){
      if (millis() - currtime > timeoutTime ){
        timeout = true;
      }
    }

    if(timeout){
      printf("timeout ACK %d\n\r", i);
    } 
    else {
      char receiveString[32];
      radio.read(&receiveString,32);
      Serial.println(receiveString);
    }

    delay(100);  
  }
#else
  while(true){
    char receiveChar[32];
    bool done = false;
    while (!done){
        done = radio.read(&receiveChar,  32);
        String receiveString = receiveChar;
        if(receiveString.substring(0,4).compareTo("ping") == 0){//oke dit is echt facking lelijk ja
          radio.stopListening();
          receiveChar[1] = 'o'; //kan niet makkelijker, er gewoon pong van maken :D
          boolean sent = radio.write(&receiveChar, 32);
          radio.startListening();
        }
    }
  }          
#endif
}

void generatePingMessage(char pingMessageInChar[], int i){
  String pingMessage = "ping ";
  pingMessage.concat(i);
  pingMessage.concat(" ");
  pingMessage.concat(millis());
  pingMessage.toCharArray(pingMessageInChar, 32);
}


