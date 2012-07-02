#include <Wire.h>
#include <Compass.h>


Compass comp;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(comp.getHeading());
  delay(20);
}
