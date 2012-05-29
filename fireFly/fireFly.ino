int someVal = 0;
boolean toggle = false;
void setup(void){
  Serial.begin(9600);
  Serial.println("hello");
  pinMode(13, OUTPUT);
  
}

void loop(){
  
  int read = analogRead(A0);
  
  unsigned long currentClock = millis();
  unsigned long newClock = currentClock;
  int i = 0;
  int lsw = 0;
  while(i < 5000){
    int read = analogRead(A0);
    if(read < 500){
      if(i > lsw){
        lsw = 0;
        Serial.println("BLEEP");
        i = calculateNewTime(i);
        lsw = i + 30;
      } 
    }else{
      newClock = millis();
      i = i + newClock - currentClock;
      currentClock = newClock;
    }
  }
  digitalWrite(13, HIGH);
  delay(20);
  digitalWrite(13, LOW);
}

int calculateNewTime(int i){
  return (int)((float) i * 1.2);
}
