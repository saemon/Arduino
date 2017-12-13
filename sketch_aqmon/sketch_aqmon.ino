#include <AqmCommons.h>
#include <AqmController.h>

AqmController ac;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  /*ac.init(
    2,3,4,
    8,7,6,
    5);*/
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    
  
}

byte i=0;
void loop() {  
  spin();
}

void spin(){      
    if(i==0){
      digitalWrite(2,HIGH);    
    }else{
      digitalWrite(2,LOW);
    }
    digitalWrite(4,HIGH);    
    delayMicroseconds(10);
    digitalWrite(4,LOW); 

    digitalWrite(3,HIGH);    
    delayMicroseconds(10);
    digitalWrite(3,LOW);
    i++;
    if(i==8)i=0;

    delay(1000);
}  



