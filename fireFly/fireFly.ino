int flashes = 0;
boolean toggle = false;
boolean isMaster = false;
void setup(void){
  Serial.begin(9600);
  Serial.println("hello");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  
  pinMode(3, INPUT);
  
  digitalWrite(13, HIGH);
  delay(20);
  digitalWrite(13, LOW);
  
  if(!digitalRead(3)){
    isMaster = true;
  }
  
}


void loop(){
  
  int read = analogRead(A0);
  
  unsigned long currentClock = millis();
  unsigned long newClock = currentClock;
  int i = 0;
  int lsw = 0;
  while(i < 5000){
    int read = analogRead(A0);
    Serial.print(i);
    Serial.print(":");
    Serial.println(read);
    if(hasFlashed(read)){
      if(i > lsw){
        lsw = 0;
        Serial.println("BLEEP");
        i = calculateNewTime(i);
        lsw = i + 30;
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
  
  if(flashes == 10){
    if(isMaster){
      digitalWrite(12, HIGH);
      delay(11000);
      digitalWrite(12, LOW);
    }else{
      digitalWrite(12, HIGH);
      delay(10000);
      digitalWrite(12, LOW);
    }
    flashes = 0;
  }
  
  flashes++;
}

int calculateNewTime(int i){
  return (int)((float) i * 1.2);
}

boolean hasFlashed(int read){
   return read < 480 || read > 540;
}
