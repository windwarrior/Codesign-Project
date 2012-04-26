#include <MMA845.h>
#include <MMA8452Q.h>
#include <I2C.h>

MMA845 mma;

#define zAxis 9
#define yellow1 3
#define yellow2 4
#define blue1 5
#define blue2 6
#define red1 10
#define red2 11
#define green1 7
#define green2 8

#define arrSize 5

int xArray[arrSize] = {blue2, blue1, zAxis, red1, red2};
int yArray[arrSize] = {green2, green1, zAxis, yellow1, yellow2};//TODO SORTERN!!!1!1!einz1!1

int x = 0, y = 0, z = 0;

void setup(){
  Serial.begin(115200);//todo: mee experimenteren
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
  mma.getAccXYZ(&x, &y, &z);
  clearHigh();
  drawAxis(x, xArray);
  drawAxis(y, yArray);
  drawZAxis(z, zAxis);
}

void drawAxis(int sensorValue, int pinArray[]){
  int mapped = map(sensorValue, -511, 511, -2, 2);
  
  if(mapped != 0){
    digitalWrite(mapped, HIGH);
  }  
}

void drawZAxis(int sensorValue, int pin){
  int mapped = map(sensorValue, -511, 511, 0, 255);
  
  analogWrite(pin, mapped);
}

void clearHigh(){
  for(int i = 0; i < arrSize; i++){
    digitalWrite(xArray[i], LOW);
    digitalWrite(yArray[i], LOW);
  }
}
