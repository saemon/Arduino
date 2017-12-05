#include <ByteIO.h>

ByteIO bio;
void setup() {
  
  Serial.begin(9600);
  byte pins[8]={2,3,4,5,6,7,8,9};
  bio.setParallel(pins);   
}

void loop() {
  bio.write(0x0F);
  delay(1000);
  bio.write(0xF0);
  delay(1000);
  // put your main code here, to run repeatedly:

}
