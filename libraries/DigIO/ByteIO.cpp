#include "Arduino.h"
#include "ByteIO.h"

ByteIO::ByteIO(){

}

boolean ByteIO::isInitialized(){
  return (bit(7)&flags==bit(7));
}

boolean ByteIO::isParallel(){
  return (bit(6)&flags==bit(6));
}

short ByteIO::setParallel(byte pins[8]){
  busPins = 0xFFFFFFFF;
  for(int i=0;i<8;i++){
    byte shf = i*4;
    busPins &= (~(0x0000000FL<<shf)) | (long(pins[i])<<shf);
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
  Serial.println(String(busPins, HEX));
  flags = bit(7)|bit(6)|flags;
  Serial.println(String(flags, BIN));
  return 1;
}


short ByteIO::write(byte b){
  for(int i=0;i<8;i++){
    byte shf = i*4;
    byte pin = (busPins>>shf) & 0x0000000FL;
    if(pin<14){
      digitalWrite(pin,bitRead(b,i));
    }
  }
}

byte ByteIO::read(){
  return 0x00;
}
