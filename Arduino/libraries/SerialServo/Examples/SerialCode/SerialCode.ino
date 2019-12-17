#include <CustServo.h>

CustServo servo (Serial);

//If baud is tied to Vdd or left open, set baud rate to 19200
//otherwise is tied to Vss and baud rate is 4800

void setup ()
{
  Serial.begin (19200); 
  servo.Number_of_Channels(4);
}  // end of setup

void loop() 
{ 
  for(int i = 0; i <= 180; i+=10)
  {
    servo.Move(1,i); // channel 1, move servo based on for loop
    delay(200);
  }
} // end of loop


