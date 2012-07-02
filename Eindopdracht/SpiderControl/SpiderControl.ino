#include <Motor.h>
#include <SpiderController.h>
#include <Servo.h>

SpiderController control(3,5,6);

void setup(){
}

void loop(){
  /*
  for(int i = mot1.getMin(); i < mot1.getMax(); i++){
    mot1.write(i);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(mot1.getPosition());
    delay(5);
  }
  
  for(int i = mot1.getMax(); i > mot1.getMin(); i--){
    mot1.write(i);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(mot1.getPosition());
    delay(5);
  }
  
  
  delay(3000);
  
  mot1.reset();
  
  delay(3000);
  
  */
  
  control.reset();
  
  delay(200);
}
