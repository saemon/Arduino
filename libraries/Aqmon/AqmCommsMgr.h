#ifndef AQM_COMMSMGR_H
#define AQM_COMMSMGR_H

#include "Arduino.h"
#include "AqmCommons.h"

#define DELAY595 1

class AqmCommsMgr{
  public:
    void init(
      short ser,
      short rclk,
      short srclk,
      short addr0,
      short addr1,
      short addr2
    );
   short setAddr(byte addr);//from 0x00 to 0x08
   short sendByte(byte data);
 private:
   short serP;
   short rclkP;
   short srclkP;
   short addr0P;
   short addr1P;
   short addr2P;
   byte lastOut;
   byte lastAddr;
};

#endif
