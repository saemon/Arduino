#include "Arduino.h"
#include "AqmCommons.h"
#include "AqmCommsMgr.h"

    void AqmCommsMgr::init(
      short ser,
      short rclk,
      short srclk,
      short addr0,
      short addr1,
      short addr2
    ){
      addr0P = addr0;
      pinMode(addr0P,OUTPUT);
      digitalWrite(addr0P,LOW);
      addr1P = addr1;
      pinMode(addr1P,OUTPUT);
      digitalWrite(addr1P,LOW);
      addr2P = addr2;
      pinMode(addr2P,OUTPUT);
      digitalWrite(addr2P,LOW);

      serP = ser;
      pinMode(serP,OUTPUT);
      digitalWrite(serP,LOW);
      rclkP = rclk;
      pinMode(rclkP,OUTPUT);
      digitalWrite(rclkP,LOW);
      srclkP = srclk;
      pinMode(srclkP,OUTPUT);
      digitalWrite(srclkP,LOW);

      lastOut = 0x00;
      lastAddr = 0x00;
    }

    short AqmCommsMgr::setAddr(byte addr){//from 0x00 to 0x07
      if(addr>7){
        return -1;
      }else{
        digitalWrite(addr0P,bitRead(addr,0));
        digitalWrite(addr1P,bitRead(addr,1));
        digitalWrite(addr2P,bitRead(addr,2));
        lastAddr=addr;
        debug("Comms Address : "+String(addr));
      }
    }

    short AqmCommsMgr::sendByte(byte data){
      //start of write bring the srclk and rclk LO to start triggerring with LO-HI edge
      digitalWrite(srclkP,LOW);
      digitalWrite(rclkP,LOW);
      for(int i=0;i<8;i++){
        //starting msb first, write bit to ser and load in with srclk
        digitalWrite(serP,bitRead(data,7-i));
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
      lastOut = data;
      debug("Comms Write :"+String(data,HEX));      
    }
