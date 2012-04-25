/* Connect MMA8452Q to Arduino Uno: 
 * VCC_IN -> 5V 
 * GND -> GND 
 * SCL -> A5
 * SDA -> A4
 * GND -> SA0
 */

#include <I2C.h>
#include "MMA845.h"

MMA845 mma;

void setup()
{

  Serial.begin(115200);
  mma.begin();
 

}
 
void loop()
{
  int x = 0, y = 0, z = 0;
 
  mma.getAccXYZ(&x, &y, &z); //get accelerometer readings in normal mode (hi res).
  //getAccXYZ(&x, &y, &z, false); //get accelerometer readings in fast mode (low res).
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
  Serial.println();
 
  delay(30);
}

