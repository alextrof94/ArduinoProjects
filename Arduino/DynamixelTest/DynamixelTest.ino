#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo

#define SERVO_ID 0x01
#define SERVO_ID2 0x02               // ID of which we will set Dynamixel too
#define SERVO_Baudrate 1000000    // Baud rate speed which the Dynamixel will be set too (57600)


void setup() {
  delay(1000);
  Dynamixel.begin(SERVO_Baudrate);
  Dynamixel.setMode(SERVO_ID, WHEEL, 0, 0);                               // set mode to WHEEL
  delay(3000);
}

void loop() {
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,i);
    Dynamixel.wheel(SERVO_ID2,LEFT,i);
    delay(1);
  }
  delay(500);
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,299 - i);
    Dynamixel.wheel(SERVO_ID2,LEFT,299 - i);
    delay(1);
  }
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,i);
    Dynamixel.wheel(SERVO_ID2,RIGHT,i);
    delay(1);
  }
  delay(500);
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,299 - i);
    Dynamixel.wheel(SERVO_ID2,RIGHT,299 - i);
    delay(1);
  }
  
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,i);
    Dynamixel.wheel(SERVO_ID2,LEFT,i);
    delay(1);
  }
  delay(500);
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,LEFT,299 - i);
    Dynamixel.wheel(SERVO_ID2,LEFT,299 - i);
    delay(1);
  }
  
  delay(500);
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,i);
    Dynamixel.wheel(SERVO_ID2,RIGHT,i);
    delay(1);
  }
  delay(500);
  for (uint16_t i = 0; i < 300; i++){
    Dynamixel.wheel(SERVO_ID,RIGHT,299 - i);
    Dynamixel.wheel(SERVO_ID2,RIGHT,299 - i);
    delay(1);
  }
  delay(500);
}
