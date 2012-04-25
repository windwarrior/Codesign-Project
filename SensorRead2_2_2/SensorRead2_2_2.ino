int led = 3;
int brightness = 0;

int analogRange = 1024;
int pwmRange = 255;
int range = 0;
void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){  
  int sensor = analogRead(A0);
  printVal(sensor);
  //Invert brightness to show higher lightvalues with higher
  //sensor values
  brightness = 255 - (sensor / (analogRange / pwmRange));
  analogWrite(led, brightness);  
}


void printVal(int sens){
  Serial.print(" Sens: ");
  Serial.print(sens);
}

