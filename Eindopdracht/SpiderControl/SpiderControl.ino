#include <Motor.h>
#include <SpiderController.h>
#include <Servo.h>

Servo serv;
SpiderController control(3,5,6);

void setup(){
  
}

void loop(){
  control.forward();  
}
