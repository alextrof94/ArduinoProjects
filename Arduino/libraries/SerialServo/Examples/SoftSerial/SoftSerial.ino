#include <CustServo.h>
#include <SoftwareSerial.h>

const byte TX = 5;

SoftwareSerial mySerial (-1, TX, false);
CustServo servo (mySerial);

void setup ()
{
  mySerial.begin (19200);
  servo.Number_of_Channels(4);
}  // end of setup

void loop () { 
  for(int i = 0; i <= 180; i+=10){
    servo.Move(1,i);
    delay(200);
  }
}


