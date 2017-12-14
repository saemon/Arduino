#include "AqmCommons.h"


void debug(String s){
  #ifdef DEBUG
    Serial.println(s);
  #endif
}
