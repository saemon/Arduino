#include "AqmCommons.h"


void debug(String s){
  #ifdef DEBUG
    Serial.println(s);
  #endif
}

void stepDelay(){
  #ifdef debug
    delay(STEPDELAY);
  #endif
}
