#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "printf.h"
#define SENDER
#define timeoutTime 200
#define HELO "helo" //handshake
#define HACK "hack" //handshake ack
#define PING "ping"
#define PONG "pong"

RF24 radio(3,9);

#define channel 67

const uint64_t pipes[2] = {0xd250dbcf39LL, 0x4aac2e23feLL};

void setup(void){
  Serial.begin(57600);
  printf_begin();
#ifdef SENDER
  printf("Will send HELO\n\r");  //we are sender
#else
  printf("Will listen for HELO\n\r");  //we are receiver
#endif

  radio.begin();

  delay(20);

  //needs to be varied during tests
  radio.setChannel(channel);
  radio.setPALevel(RF24_PA_MIN);
  //radio.setDataRate(RF24_2MBPS);
  
  radio.setRetries(0,0);
  radio.setAutoAck(false);
  radio.printDetails();
#ifdef SENDER
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  startSendingHelo();//start handshake
#else
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  startListeningForHelo();//start handshake
#endif
}

//START HANDSHAKE
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
        receivedHACK = true;
      }
    }
    delay(20);
  }

}
#else
void startListeningForHelo(void){
  boolean finished = false;
  while(!finished){
    if(radio.available()){
      bool done = false;
      char receiveChar[32];
      while (!done){
        done = radio.read(&receiveChar,  32);
        if(receiveChar[0] == 'h'){
          delay(20);

          radio.stopListening();
          char result = 'A';
          boolean sent = radio.write(&result, radio.getPayloadSize());
          radio.startListening();
          finished = true;
        }
        delay(20);
      }
    }
  }
}
#endif
//END HANDSHAKE

void loop(void){
#ifdef SENDER
  for(int i = 0; i < 100; i++){
    char msg[32];
    generatePingMessage(msg, i);    
    Serial.println(msg);
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
      Serial.print("current time ");
      Serial.print(millis());
      Serial.println();
    }

    delay(100);  
  }
  exit(0);
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
  String pingMessage = PING;//"ping ";
  pingMessage.concat(i);
  pingMessage.concat(" ");
  pingMessage.concat(millis());
  pingMessage.toCharArray(pingMessageInChar, 32);
}


