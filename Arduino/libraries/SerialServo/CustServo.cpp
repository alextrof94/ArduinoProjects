//CustServo.cpp

#include "CustServo.h"

void CustServo::Number_of_Channels(uint8_t channel)
{
  switch(channel)
  {
	case 10:
	case 23:
	case 34:
	  left = 15; right = 240;
	  break;
	  
	default: // channel = 4
	  left = 7; right = 240;
  }
  
  for(int i = 0; i < (channel + 1); i++)
    ServoNum[i] = i + 48;
}

void CustServo::Move(uint8_t chan, uint8_t position)
{ time = 0;
  port_.write (ServoNum[chan]);            // What channel to use   
  port_.write (map(position,0,179,left,right)); // send position command
  delay(10);
  // time = millis();
  // while(millis() - time < 10) {;}	
} 

void CustServo::Calibrate()
{
  time = 0;
  time = millis();                          //set check time
	
  for (int j = 1; j <= 5; j++)
  { 
    port_.write(48);                  //time check null channel
    port_.write(left);                   //check 0 degree
  
    while(millis() - time < 1000) { ; }
    time = millis();                        // set time again
	  
    port_.write(48);                  
    port_.write(127);                 //check 90 degree "CENTER"
	  
    while(millis() - time > 1000) { ; }
	  
    port_.write(48);                  
    port_.write(right);                 //check 180 degree
  }
}