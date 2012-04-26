int pinArray[6] = {2,3,4,5,6,7};
int minSensor = 1023;
int maxSensor = 0;
void setup(){
  Serial.begin(9600);
  for(int i = 0; i < 6; i++){
    pinMode(pinArray[i], OUTPUT);
  }
}

void loop(){
  int sensor = analogRead(A0);
  Serial.println(sensor);
  
  if(sensor < minSensor){
    minSensor = sensor;
  }
  
  if(sensor > maxSensor){
     maxSensor = sensor; 
  }
  int mapped = map(sensor, minSensor, maxSensor, 0, 6);
  
  clearHigh();
  
  for(int i = 0; i < mapped; i++){
    digitalWrite(pinArray[i], HIGH);
  }
}

void clearHigh(){
  for(int i = 0; i < 6; i++){
    digitalWrite(pinArray[i], LOW);
  }
}
