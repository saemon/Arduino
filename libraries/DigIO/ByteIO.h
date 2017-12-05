#ifndef BYTEIO_H
#define BYTEIO_H

#include "Arduino.h"

class ByteIO{
  public:
    ByteIO();
    short setParallel(byte pins[8]);
    short write(byte b);
    byte read();
    boolean isParallel();
    boolean isInitialized();
    short setPins(byte pins[8]);
    short setPins(long pins);
    long getPins();
  private:
    /*read write mode
    0=UNDEF
    1=SERIAL
    2=PARALLEL
    */
    byte flags=0;
    long busPins;
};
#endif
