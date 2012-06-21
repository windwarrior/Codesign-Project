#include <I2C.h>
#include <MMA8452Q.h>
#include <MMA845.h>
#include "mmaCal.h"
//#define debug
MMA845 mma;

int xCenter = 0, yCenter = 0, zCenter = 0;

int xGain = 0, yGain = 0, zGain = 0;

int iterations = 20;


void setup(){
  pinMode(13, OUTPUT);
  mma.begin();
  Serial.begin(115200);
  delay(2000);
  calibrate();
}

void calibrate(){


  int xTot = 0, yTot = 0, zTot = 0;

  int xMin = 5000, yMin = 5000,  zMin = 5000;
  
  int xMax = 0, yMax = 0, zMax = 0;
  
  digitalWrite(13, HIGH);
  for(int i = 0; i<iterations; i++){
    int x = 0 , y =  0, z = 0;
    mma.getAccXYZ(&x,&y,&z);
    x = map(x, -512, 511, 0, 1023);
    y = map(y, -512, 511, 0, 1023);
    z = map(z, -512, 511, 0, 1023);
    xTot = xTot + x;
    yTot = yTot + y;
    zTot = zTot + z;
    #ifdef debug
    Serial.print("xTot: ");
    Serial.print(xTot);
    Serial.print(" x: ");
    Serial.print(x);
    Serial.print("yTot: ");
    Serial.print(yTot);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print("zTot: ");
    Serial.print(zTot);
    Serial.print(" z: ");
    Serial.print(z);
    #endif
    delay(100);

    xMin = min(x, xMin);
    yMin = min(y, yMin);
    zMin = min(z, zMin);
    #ifdef debug
    Serial.print(" xMin: ");
    Serial.print(xMin);
    Serial.print(" yMin: ");
    Serial.print(yMin);
    Serial.print(" zMin: ");
    Serial.println(zMin);    
    Serial.println("--------------------------");
    #endif
    xMax = max(x, xMax);
    yMax = max(y, yMax);
    zMax = max(z, zMax);


  }
  digitalWrite(13, LOW);

  xCenter = map(xTot / iterations, 0, 1023, -512, 512);
  yCenter = map(yTot / iterations, 0, 1023, -512, 512);
  zCenter = map(zTot / iterations, 0, 1023, -512, 512);

  xGain = xMax - xMin;
  yGain = yMax - yMin;
  zGain = zMax - zMin;

  Serial.print("Calibrated, x center: ");
  Serial.print(xCenter);
  Serial.print(" y center: ");
  Serial.print(yCenter);
  Serial.print(" z center ");
  Serial.println(zCenter);
  Serial.print("Gain, x gain: ");
  Serial.print(xGain);
  Serial.print(" y gain: ");
  Serial.print(yGain);
  Serial.print(" z gain: ");
  Serial.println(zGain);
  delay(20);
}

void loop(){

  int x, y, z = 0;
  mma.getAccXYZ(&x,&y,&z);
  #ifdef debug
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" xCorr: ");
  Serial.print(x - xCenter);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" yCorr: ");
  Serial.print(y - yCenter);
  Serial.print(" ");
  Serial.print(z);
  Serial.print(" zCorr: ");
  Serial.print(z - zCenter);
  Serial.print(" dir: " );
  #endif
  Direction dir = getDirection(x - xCenter, y - yCenter);
  if(dir == FORWARD){
    Serial.println("Forward" );
  }else if(dir == BACK){
    Serial.println("Back" );
  }else if(dir == LEFT){
    Serial.println("Left" );
  }else if(dir == RIGHT){
    Serial.println("Right" );
  }else{
    Serial.println("None");
  }
  delay(20);
}

Direction getDirection(int x, int y){
  if(x > 200){
    return FORWARD;
  }else if(x < -200){
    return BACK;
  }else if(y > 200){
    return RIGHT;
  }else if(y < -200){
    return LEFT;
  }else{
    return NONE;
  }
}




