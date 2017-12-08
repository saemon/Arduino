#include "AqmonCommons.h"
#include "AqmonSipo595.h"

void AqmonSipo595::init(byte serPin, byte oePin, byte rclkPin, byte srclkPin, byte srclrPin){
  debug("AqmonSipo595 Init with pins {"+String(serPin)+","+String(oePin)+","+String(rclkPin)+","+String(srclkPin)+","+String(srclrPin)+"}");
  serP = serPin;
  oeP = oePin;
  rclkP = rclkPin;
  srclkP = srclkPin;
  srclrP = srclrPin;
  isInit = true;
  pinMode(serP,OUTPUT);
  pinMode(oeP,OUTPUT);
  pinMode(rclkP,OUTPUT);
  pinMode(srclkP,OUTPUT);
  pinMode(srclrP,OUTPUT);

  digitalWrite(serP,LOW);
  digitalWrite(oeP,LOW);
  digitalWrite(rclkP,LOW);
  digitalWrite(srclkP,LOW);
  digitalWrite(srclrP,HIGH);

}

short AqmonSipo595::write1(byte b){
  //start of write bring the srclk and rclk LO to start triggerring with LO-HI edge
  digitalWrite(srclkP,LOW);
  digitalWrite(rclkP,LOW);
  for(int i=0;i<8;i++){
    //starting msb first, write bit to ser and load in with srclk
    digitalWrite(serP,bitRead(b,7-i));
    //introduce delays to ensure sufficient propagation time.
    delayMicroseconds(DELAY595);
    digitalWrite(srclkP,HIGH);
    delayMicroseconds(DELAY595);
    digitalWrite(srclkP,LOW);
  }
  //send to storage
  delayMicroseconds(DELAY595);
  digitalWrite(rclkP,HIGH);
  delayMicroseconds(DELAY595);
  digitalWrite(rclkP,LOW);
  curOut=((curOut<<8)|b);
  debug("write1 :"+String(b,HEX));
}

short AqmonSipo595::write2(unsigned int i){
  debug("write2 :"+String(i,HEX));
  byte b1 = lowByte(i>>8);
  write1(b1);
  byte b0 = lowByte(i);
  write1(b0);
}

short AqmonSipo595::write4(unsigned long l){
  debug("write4 :"+String(l,HEX));
  byte b3 = lowByte(l>>24);
  write1(b3);
  byte b2 = lowByte(l>>16);
  write1(b2);
  byte b1 = lowByte(l>>8);
  write1(b1);
  byte b0 = lowByte(l);
  write1(b0);  
}

word AqmonSipo595::getCurOut(){
  return word(curOut);
}
