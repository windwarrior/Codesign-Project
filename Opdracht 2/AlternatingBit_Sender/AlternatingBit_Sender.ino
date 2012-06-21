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
  radio.setRetries(0,0);
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
  char seqChar = seq ? '1' : '0';
  generatePingMessage(msg, seqChar, globSeq);
  msg[31] = (char) 0x00;
  bool isSend = radio.write(msg, 32);
  Serial.print("Sent package ");
  Serial.print(msg);
  if(isSend){
    Serial.println(" succes");
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
      
      if(result[0] == seqChar){
        seq = !seq;
        globSeq++;
        
        if(globSeq == 1000){
          delay(200);
          exit(0);
        }
      }
    }

  }
  else{
    Serial.println(" failed");
  }
  radio.startListening();
  delay(100);

}

void generatePingMessage(char pingMessageInChar[], char abc, int globSeq){
  String pingMessage = "";//"ping ";
  pingMessage.concat(abc);
  pingMessage.concat(" PNG ");
  pingMessage.concat(globSeq);
  pingMessage.toCharArray(pingMessageInChar, 32);
}








