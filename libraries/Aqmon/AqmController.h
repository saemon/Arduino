#ifndef AQM_CONTROLLER_H
#define AQM_CONTROLLER_H

#include "AqmCommons.h"
#include "AqmCommsMgr.h"
#include "AqmLCD.h"

#define ADDR_LCD  0x03
#define ADDR_BRD  0x06


class AqmController{
  public:
    void init(
      short ser, // pin connected to SER of 74595
      short rclk, // pin connected to RCLK of 74595
      short srclk, // pin connected to SRCLK of 74595
      short addr0,
      short addr1,
      short addr2,
      short pwm1 // pin for 1st pwm signal
    );
    short loop();
  private:
    byte mutex = 0x00;
    AqmCommsMgr comms;
    AqmLCD lcd;
};


#endif
