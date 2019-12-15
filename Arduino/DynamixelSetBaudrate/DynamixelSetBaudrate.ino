#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo

#define SERVO_ID 0x02            // ID of which we will set Dynamixel too
#define SERVO_Baudrate 200000    // Baud rate speed which the Dynamixel will be set too (57600)
#define SERVO_NEW_Baudrate 1000000    // Baud rate speed which the Dynamixel will be set too (57600)

void setup() {
  delay(1000);
  Dynamixel.begin(SERVO_Baudrate);
  delay(1000);
  Dynamixel.ledState(SERVO_ID, ON);
  delay(1000);
  Dynamixel.ledState(SERVO_ID, OFF);
  delay(1000);
  Dynamixel.setBaudRate(SERVO_ID, SERVO_NEW_Baudrate);
  Dynamixel.begin(SERVO_NEW_Baudrate);
  delay(1000);
}

void loop() {
  Dynamixel.ledState(SERVO_ID, ON);
  delay(100);
  Dynamixel.ledState(SERVO_ID, OFF);
  delay(100); 
}
