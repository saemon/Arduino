#include <AqmCommons.h>
#include <AqmController.h>

AqmController ac;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ac.init(
    2,3,4,
    8,7,6,
    5);
    
  
}

void loop() {  
  ac.loop();
  
}




