/*
J.Teda 21/04/2013

Exsample of how to run Dynamixel in WHEEL mode - tell servo to move continues at speed

Robotis e-Manual ( http://support.robotis.com )

*/

#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo

#define SERVO_ID 0x02               // ID of which we will set Dynamixel too
//#define SERVO_ControlPin 0x02       // Control pin of buffer chip, NOTE: this does not matter becasue we are not using a half to full contorl buffer.
#define SERVO_Baudrate 200000    // Baud rate speed which the Dynamixel will be set too (57600)


void setup(){
 delay(1000);                                                           // Give time for Dynamixel to start on power-up

  Dynamixel.begin(SERVO_Baudrate);  
//  Dynamixel.setDirectionPin(SERVO_ControlPin);                            // Optional. Set direction control pin
  Dynamixel.setMode(SERVO_ID, WHEEL, 0, 0);                               // set mode to WHEEL

  delay(3000);
}


void loop(){
  Dynamixel.ledState(SERVO_ID, ON);
  for (uint16_t i = 0; i < 1024; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,i);
    delay(1);
  }
  for (uint16_t i = 0; i < 1024; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,1023 - i);
    delay(1);
  }
  
  Dynamixel.ledState(SERVO_ID, OFF);
  for (uint16_t i = 0; i < 1024; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,i);
    delay(1);
  }
  for (uint16_t i = 0; i < 1024; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,1023 - i);
    delay(1);
  }
}
