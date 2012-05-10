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
uint64_t baseadress = 0xd250dbcf00ll;/*doe hier iets mee*/
byte hopToSender = 0xab;/*doe hier iets mee*/
byte hopToReceiver = 0xcd; /*doe hier iets mee*/
byte hopReadingPipe = 0xef; /*doe hier iets mee*/
void setup(void){
  Serial.begin(57600);
  radio.begin();
  delay(20);

  radio.setChannel(channel);
  radio.printDetails();
  
  #ifdef SND
  radio.openReadingPipe(1, generateAddress(hopToSender));
  radio.openWritingPipe(generateAddress(hopReadingPipe));
  #endif
  
  #ifdef HOP
  radio.openReadingPipe(2, generateAddress(hopReadingPipe));
  #endif
  
  #ifdef RCV
  radio.openReadingPipe(3, generateAddress(hopToReadingPipe));
  radio.writingPipe(generateAddress(hopReadingPipe));
  #endif

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

uint64_t generateAddress(byte addr){
  return (baseadress & ~0xff) | addr;
}




