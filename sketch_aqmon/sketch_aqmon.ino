#include <AqmonCommons.h>
#include <AqmonController.h>

AqmonController ac;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ac.init(
    13,12,11,10,9,
    16,1,4,0,
    0,
    0);
  
}

void loop() {
  ac.loop();
}
