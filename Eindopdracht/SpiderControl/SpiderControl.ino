#include <Motor.h>
#include <SpiderController.h>
#include <Servo.h>

SpiderController control;

void setup(){
  Serial.begin(9600);
  control.begin(3,5,6);
}

void loop(){

  for(int i=0; i<9; i++){
    control.forward();
    Serial.println("testing forward");
  }
  control.turnLeft();
  Serial.println("testing turnLeft");
    
  for(int i=0; i<9; i++){
    control.forward();
    Serial.println("testing forward");
  }
  
  control.turnRight();
  Serial.println("testing turnRight");

}
