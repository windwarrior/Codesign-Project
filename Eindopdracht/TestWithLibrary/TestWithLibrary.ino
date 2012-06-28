#include <Servo.h>

#include <SpiderController.h>



SpiderController control(9,10,11);

void setup(){
  
}

void loop(){
  control.forward();
  control.turnLeft();
  control.turnRight();
  control.back();
}
