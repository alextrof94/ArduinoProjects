/*
J.Teda 21/04/2013

This is a smiple ping test to see if your Half to full duplex circuit is working using the LED on pin 13
LED ON = Dynamixel PING successful
LED OFF = Dynamixel PING fail

You well need to make a half duplex to full duplex circuit to run this sketch. ( http://support.robotis.com/en/product/dynamixel/dxl_mx_main.htm )
Robotis e-Manual ( http://support.robotis.com )

*/

#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo

#define SERVO_ID 0x05               // ID of Dynamixel (0x01 is default)
#define SERVO_ControlPin 4       // Control pin of buffer chip ( 74LS241N )
#define SERVO_SET_Baudrate 1000000    // Baud rate speed which the Dynamixel will be set too (57600 is default)
#define LED13 0x0D                  // Pin for Visual indication when ping test is successful - pin 13 normal has a built in LED on Arduino UNO


void setup(){

  pinMode(4, OUTPUT);
  digitalWrite(4, 1);
  pinMode(LED13, OUTPUT);
  digitalWrite(LED13, LOW);

  delay(1000);                                               // Give time for Dynamixel to start on power-up

  Dynamixel.begin(Serial1, 1000000);                       // Set up Arduino to communicate to Dynamixel
  Dynamixel.setDirectionPin(SERVO_ControlPin);               // Optional. Set direction control pin
  Serial.begin(57600);
}


void loop() {
  Dynamixel.ledState(SERVO_ID, 1);
  delay(1);
  digitalWrite(LED13, HIGH);
  if (Dynamixel.ping(SERVO_ID) == SERVO_ID){ 
    Serial.println("OK");
  }else{
    Serial.println("NOT OK");
  }
  
  delay(100);
  digitalWrite(LED13, LOW);
  Dynamixel.ledState(SERVO_ID, 0);

  delay(500);  // Delay 0.5 Sec
}
