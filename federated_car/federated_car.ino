#include "car.h"


void setup() {
  Serial.begin(9600);
   while (!Serial) {}
  trainingMode();

}



void loop() {
  // put your main code here, to run repeatedly:
  _loop();
}
