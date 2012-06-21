int flashes = 0;
boolean isMaster = true;

char linear = 'l';
char squared = 's';

void setup(void){
  Serial.begin(9600);
  Serial.println("hello");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);


  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  
  pinMode(3, INPUT);

}

void loop(){
//  doTest(linear, 1.1, 30, 10);
//  doDelay(2500);
   doTest(linear, 1.2, 30, 10);
   
   doDelay(3000);
   
   doTest(linear, 1.2, 5, 40);  
//  doTest(squared, (1.0f/5000.0f), 30, 30);
//  doTest(squared, (1/2500), 30, 10);
//  doDelay(1000);
//  doTest(squared, (1/5000), 30, 10);
//  doDelay(1000);
  
  delay(10000);
}

void doTest(char type, float functionargument, int lswAmount, int flashes){
  Serial.print("Doing test: ");
  Serial.print(type);
  Serial.print(" with argument ");
  Serial.print(functionargument);
  Serial.print(" and lsw of ");
  Serial.println(lswAmount);
  for(int i = 0; i < flashes; i++){
    int read = analogRead(A0);
    unsigned long currentClock = millis();
    unsigned long newClock = currentClock;
    int i = 0;
    int lsw = 0;
    while(i < 5000){
      int read = analogRead(A0);
      if(i%10 == 0){
        Serial.print(i);
        Serial.print(":");
        Serial.println(read);
      }
      if(hasFlashed(read)){
        if(i > lsw){
          lsw = 0;
          Serial.println("BLEEP");
          if(type == linear){
            i = calculateNewTimeLinear(i, functionargument); 
          }else if(type == squared){
            i = calculateNewTimeSquared(i, functionargument);
          }else{
            Serial.println("What the fack!");
          }
          lsw = i + lswAmount;
        }
      }
      newClock = millis();
      i = i + newClock - currentClock;
      currentClock = newClock;
    }
    digitalWrite(13, HIGH);
    Serial.println("Blinking");
    delay(20);
    digitalWrite(13, LOW);
  }
}

void doDelay(int del){
  digitalWrite(12, HIGH);
  if(isMaster){
    delay(del + 10000);
  }else{
    delay(10000);
  }
  digitalWrite(12, LOW);
}
int calculateNewTimeLinear(int i, float constant){
  return (int)((float) i * constant);
}


int calculateNewTimeSquared(int i, float constant){
  return (int)(constant * i * i);
}

boolean hasFlashed(int read){
  return read < 480 || read > 540; //voor geluid
  //return read > 100; //voor licht
}

