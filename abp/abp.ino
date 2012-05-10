#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#define channel 67
#define HOP
//#define SND
//#define RCV

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
#ifdef HOP //hop
  radio.startListening();
  //bool timeout = false;
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
  radio.read(&readstring, 32);//TODO reading pipe is niet geopend?
  radio.openWritingPipe(sendTo);
  radio.write(&readstring, 32);
  
#elif RCV //receiver
  //open de juiste pipes hier, deze heeft maar 2 pipes (read/write naar hop)
   
  radio.startListening();
  
  if(radio.available()){
    bool done = false;
    char receiveChar[32];
    while(!done){
      done = radio.read(&receiveChar, 32);
      radio.stopListening();
      char result = 'A';//de ack moet ook nog het seq number in die in receiveChar zit
      boolean sent = radio.write(&result, radio.getPayloadSize());
    }
  }
  //en weer loop
  
#elif SND //sender
  //open de juiste pipes hier, deze heeft maar 2 pipes (read/write naar hop)

#endif
}






