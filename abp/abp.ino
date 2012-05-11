#include <SPI.h>

#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include "printf.h"
#define channel 67
//ONLY DEFINE 1
#define HOP
//#define SND
//#define RCV
RF24 radio(3,9);
uint64_t baseadress = 0x1761d0f640ll;/*doe hier iets mee*/
byte hopToSender = 0xab;/*doe hier iets mee*/
byte hopToReceiver = 0xcd; /*doe hier iets mee*/
byte hopReceiverReadingPipe = 0xef; /*doe hier iets mee*/
byte hopSenderReadingPipe = 0x33;
uint64_t hopToSenderAddr = 0x1761d0f64all; //generateAddress(hopToSender);
uint64_t hopToReceiverAddr = 0x1761d0f64bll; //generateAddress(hopToReceiver);
uint64_t hopReceiverReadingPipeAddr = 0x1761d0f64cll; //generateAddress(hopReceiverReadingPipe);
uint64_t hopSenderReadingPipeAddr = 0x1761d0f64dll; //generateAddress(hopSenderReadingPipe);

uint8_t HOP_SENDER = 2;
uint8_t HOP_RECEIVER = 3;
boolean seq = 0;//huidig sequence nummer
boolean receivedAck = false;  
unsigned int globSeq = 0;

void setup(void){
  Serial.begin(57600);
  Serial.println("BLIEEEEEEP");
  printf_begin();
  radio.begin();
  delay(20);

  radio.setChannel(channel);
  

#ifdef SND
  radio.openReadingPipe(1, hopToSenderAddr);
  radio.openWritingPipe(hopSenderReadingPipeAddr);
#endif

#ifdef HOP
  Serial.println("bliep");
  radio.openReadingPipe(HOP_SENDER, hopSenderReadingPipeAddr);
  radio.openReadingPipe(HOP_RECEIVER, hopReceiverReadingPipeAddr);\
  radio.openWritingPipe(hopToSenderAddr);
#endif

#ifdef RCV
  radio.openReadingPipe(4, hopToReceiverAddr);
  radio.openWritingPipe(hopReceiverReadingPipeAddr);
#endif

  radio.printDetails();
}

void loop(void){
#ifdef HOP //hop
  //bool timeout = false;
  boolean ready = false;
  uint64_t sendTo =  0;
  while (!ready){
    if(radio.available(&HOP_SENDER)){
      ready = true;
      sendTo = hopToReceiverAddr;
      Serial.println("Got message from sender, sending to receiver");
    }

    if(radio.available(&HOP_RECEIVER)){
      ready = true;
      sendTo = hopToSenderAddr;
      Serial.println("Got message from receiver, sending to sender");
    }  
  }
  char readstring[32];
  radio.stopListening();
  radio.read(&readstring, 32);//TODO reading pipe is niet geopend?
  Serial.println(readstring);
  radio.startListening();
//  radio.openWritingPipe(sendTo);
//  radio.write(&readstring, 32);
#endif 

#ifdef RCV //receiver
  //open de juiste pipes hier, deze heeft maar 2 pipes (read/write naar hop)

  radio.startListening();

  if(radio.available()){
    bool done = false;
    char receiveChar[32];
    while(!done){
      done = radio.read(&receiveChar, 32);
      radio.stopListening();
      //de ack moet ook nog het seq number in die in receiveChar zit
      boolean sent = radio.write(&receiveChar, radio.getPayloadSize());
    }
  }
  //en weer loop
#endif

#ifdef SND //sender
  //open de juiste pipes hier, deze heeft maar 2 pipes (read/write naar hop)
  /*
  send some package
   while not recieved ack
   timeout got
   resend
   
   flip bit
   globseq++;
   */
  char msg[32];
  String msgString = seq == 0 ? "0" : "1";
  msgString.toCharArray(msg, 32);
  Serial.print(msg);
  boolean succes = radio.write(&msg, 32);
  if(succes){
    Serial.println(" succes");
    boolean gotAck = false;
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && !timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;
    if(!timeout){
      char result[32];
      radio.read(&result, 32);
      //process that shit
      //WARNING: scary code incoming
      char stringSet[6][10];

      int currentSplitString = 0;
      int charInCurrentString = 0;
      //Initialiseer de array op null bytes
      for(int k = 0; k < 6; k++){
        stringSet[k][0] = (byte) 0x00;
      }
      //Lees de string char voor char in
      for(int i = 0; i < sizeof(result); i++){
        char currentChar = result[i];
        //Als het de laatste char is, 
        //termineer dan ook de laatste string
        if(currentChar == (char) 0x00){
          stringSet[currentSplitString][charInCurrentString] = (byte) 0x00;
        }
        //Als de char niet het split char is
        //dan vul je de huidig geselecteerde array
        //aan met de character
        else if(currentChar != ' '){
          stringSet[currentSplitString][charInCurrentString] = currentChar;
          charInCurrentString++;
        }
        //Als de chat wel het split char is
        //termineer dan de huidig geselecteerde array
        //en verhoog dan de array pointer
        else{
          stringSet[currentSplitString][charInCurrentString] = (byte) 0x00;
          currentSplitString++;
          charInCurrentString = 0;
        }


      }
      //END OF WARNING
      char seqChar = seq == 0 ? '0' : '1';
      if(stringSet[0][0] == seqChar){//DIT GAAT VAST STUK HIER
        //hele string outputten en checken of de globSeq nog gelijk i
        Serial.println(result);
        seq = !seq;
        globSeq++;
      }
      radio.startListening();
      delay(20);
    }
    else{
      //Radio timeout
    }
    radio.startListening();
  }
  else{
    //Backoff timer
    Serial.println(" fail");
    delay(random(200,500));
  } 
#endif
}

uint64_t generateAddress(byte addr){
  return (baseadress & ~0xff) | addr;
}






