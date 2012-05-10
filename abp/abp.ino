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

boolean seq = 0;//huidig sequence nummer
boolean receivedAck = false;  
unsigned int globSeq = 0;
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
  /*
  send some package
   while not recieved ack
   timeout got
   resend
   
   flip bit
   globseq++;
   */
  radio.stopListening();
  char msg[32];
  boolean succes = radio.write(&msg, 32);
  if(succes){
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
        seq = !seq;
        globSeq++;
      }
    }
    else{
      //Radio timeout
    }
  }
  else{
    //Backoff timer
    delay(random(20,50));
  } 


#endif
}

uint64_t generateAddress(byte addr){
  return (baseadress & ~0xff) | addr;
}






