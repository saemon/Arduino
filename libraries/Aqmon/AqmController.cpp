#include "AqmCommons.h"
#include "AqmCommsMgr.h"
#include "AqmLCD.h"
#include "AqmController.h"


void AqmController::init(
    short ser, // pin connected to SER of 74595
    short rclk, // pin connected to RCLK of 74595
    short srclk, // pin connected to SRCLK of 74595
    short addr0,
    short addr1,
    short addr2,
    short pwm1 // pin for 1st pwm signal
  ){
  comms.init(ser,rclk,srclk,addr0,addr1,addr2);
  comms.setAddr(ADDR_LCD);
  //lcd.init(&comms);
  //lcd.begin(16,2,0x00);
  //lcd.clear();
  //lcd.write("Welcome");

}

short AqmController::loop(){
  delay(5000);
  return 1;
}
