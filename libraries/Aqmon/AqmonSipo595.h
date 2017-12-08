#ifndef AQMON_SIPO595_H
#define  AQMON_SIPO595_H

#include "AqmonCommons.h"

#define DELAY595 1 //1us is sufficient as 74hc595 delays are in ns

class AqmonSipo595{
  public:
    short write1(byte b);
    short write2(unsigned int i);
    short write4(unsigned long l);
    void init( byte serPin, byte oePin, byte rclkPin, byte srclkPin, byte srclrPin);
    word getCurOut();
  private:
    boolean isInit = false;
    unsigned long curOut = 0x00000000;
    byte serP = 0xFF;
    byte oeP = 0xFF;
    byte rclkP = 0xFF;
    byte srclkP = 0xFF;
    byte srclrP = 0xFF;
};
#endif
