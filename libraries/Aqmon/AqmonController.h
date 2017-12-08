#ifndef AQMON_CONTROLLER_H
#define AQMON_CONTROLLER_H

#include "AqmonCommons.h"
#include "AqmonSipo595.h"
#include "AqmonLCD.h"

#define IDEN_LCD  0x01
#define IDEN_DMUX1 0x02


class AqmonController{
  public:
    void init(
      byte sipoSer, // pin connected to SER of 74595
      byte sipoOe, // pin connected to OE of 74595
      byte sipoRclk, // pin connected to RCLK of 74595
      byte sipoSrclk, // pin connected to SRCLK of 74595
      byte sipoSrclr, // pin connected to SRCLR of 74595
      byte lcdCols,  // no of columns supported by LCD
      byte lcdRows, // no of rows supported by LCD
      byte lcdMode, //4 ot 8 bit lcd data bus
      byte lcdBusOffset, //bitmask for which 5 bits of sipo bus drive the LCD, must be contiguos and in LCD pin order
      byte dmux1BusOffset, //bitmask for which 3 bits of sipo bus drive the 8channel dmux1, must be contiguos and in dmux pin order
      byte dmux2BusOffset //bitmask for which 3 bits of sipo bus drive the 8channel dmux2, must be contiguos and in dmux pin order
    );
    short loop();
  private:
    byte mutex = 0x00;
    word instrWord = 0x0000;
    word statWord = 0x0000;
    word ctrlWord = 0x0000;
    byte lcdBusOffset = 0x00; //bitmask for which 5 bits of sipo bus drive the LCD, must be contiguous and in LCD pin order
    AqmonSipo595 sipo;
    AqmonLCD lcd;
};



#endif
