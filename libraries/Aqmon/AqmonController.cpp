#include "AqmonCommons.h"
#include "AqmonSipo595.h"
#include "AqmonLCD.h"
#include "AqmonController.h"


void AqmonController::init(
  byte sipoSer, // pin connected to SER of 74595
  byte sipoOe, // pin connected to OE of 74595
  byte sipoRclk, // pin connected to RCLK of 74595
  byte sipoSrclk, // pin connected to SRCLK of 74595
  byte sipoSrclr, // pin connected to SRCLR of 74595
  byte lcdCols,  // no of columns supported by LCD
  byte lcdRows, // no of rows supported by LCD
  byte lcdMode, //4 ot 8 bit lcd data bus
  byte lcdBusOffset, //offset for which 5 bits of sipo bus drive the LCD, assumed to be contiguos and in LCD pin order
  byte dmux1BusOffset, //offset for which 3 bits of sipo bus drive the 8channel dmux1, assumed to be contiguos and in dmux pin order
  byte dmux2BusOffset //offset for which 3 bits of sipo bus drive the 8channel dmux2, assumed to be contiguos and in dmux pin order
){
  sipo.init(sipoSer,sipoOe,sipoRclk,sipoSrclk,sipoSrclr);
  sipo.write2(0x0000);
  lcd.init(16,1,lcdMode,lcdBusOffset,&sipo);
  //lcdInit(lcdCols,lcdRows,lcdMode,lcdBusOffset);
  //dmx1.busOffset = dmux1BusOffset;
  //dmx2.busOffset = dmux2BusOffset;
  lcd.begin(16,2,0x00);
  lcd.write("Welcome");
}

short AqmonController::loop(){
  delay(500);
  return -1;
}
