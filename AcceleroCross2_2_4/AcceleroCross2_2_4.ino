#include <MMA845.h>
#include <MMA8452Q.h>
#include <I2C.h>

MMA845 mma;

//pinnen mappen
#define zAxis 9
#define yellow1 3
#define yellow2 4
#define blue1 5
#define blue2 6
#define red1 10
#define red2 11
#define green1 7
#define green2 8

//resolutie van de x en y as
#define arrSize 5
#define arrMid 2

//map de pinnen naar een as
int xArray[arrSize] = {blue2, blue1, zAxis, green1, green2};
int yArray[arrSize] = {yellow2, yellow1, zAxis, red1, red2};

int x = 0, y = 0, z = 0;

//offset voor de zelfcalibrerende resolutie
//Dus als je uiterste waarden van de sensor
//hoger worden, dan veranderd de schaal
//van de (x&y) en z-as
int horOffset = 0;//offset x, y-as
int verOffset = 0;//offset z-as

void setup(){
  Serial.begin(115200);
  mma.begin();
  
  pinMode(zAxis, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(blue1, OUTPUT);
  pinMode(blue2, OUTPUT);
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
}

void loop(){
  //accelerometer uitlezen
  mma.getAccXYZ(&x, &y, &z);
  
  //ledjes uitzetten
  clearHigh();
  
  //accelerometer waarden printen (voor processing)
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println();
  
  //offset x,y aanpassen
  if(abs(x) > horOffset){
    horOffset = abs(x);
    
  }else if(abs(y) > horOffset){
    horOffset = abs(y);
  }
  
  //offset z aanpassen
  if(abs(z) > verOffset){
    verOffset = abs(z);
  }
  
  //ledjes aansturen
  drawAxis(x, xArray);
  drawAxis(y, yArray);
  drawZAxis(z, zAxis);
 
  delay(30);
}

void drawAxis(int sensorValue, int pinArray[]){
  //De schaal van de sensorValue aanpassen naar de grootte van de array van ledjes.
  //Aan de hand van de horizontale offset die bepaald wordt door de maximale sensorwaarde
  //als bovengrens in te stellen.
  int mapped = map(sensorValue + 511, 0 + (512-horOffset), 511 + horOffset, 0, 5);
  
  //Juiste leds aanzetten vanaf het midden tot en met de mapped value.
  //De middelste led wordt hier niet aangestuurd. Deze wordt met PWM voor de z-as gebruikt.
  if(mapped < arrMid){
    for(int i = 1; i >= mapped; i--){
      digitalWrite(pinArray[i], HIGH);
    }
  }else if(mapped > arrMid){
    for(int i = 3; i <= mapped; i++){
      digitalWrite(pinArray[i], HIGH);
    }
  }
}

void drawZAxis(int sensorValue, int pin){
  //De schaal van de sensorValue aanpassen naar het bereik van de PWM pin.
  //Aan de hand van de verticale offset die bepaald wordt door de maximale sensorwaarde
  //als bovengrens in te stellen.
  int mapped = map(sensorValue + 512, 512-verOffset, 511 + verOffset, 0, 255);
  //Serial.println(mapped);
  analogWrite(pin, mapped);
}

void clearHigh(){
  //Alle leds uitzetten
  for(int i = 0; i < arrSize; i++){
    digitalWrite(xArray[i], LOW);
    digitalWrite(yArray[i], LOW);
  }
}
