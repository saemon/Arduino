#include "AqmonCommons.h"


void debug(String s){
  #ifdef DEBUG
    Serial.println(s);
  #endif
}
