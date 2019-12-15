#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo

#define SERVO_Baudrate 1000000    // Baud rate speed which the Dynamixel will be set too (57600)


void setup() {
  delay(1000);
  Dynamixel.begin(Serial1, SERVO_Baudrate);
  for (int i = 1; i < 7; i++){
    Dynamixel.setMode(i, 1, 0x001, 0xFFF);
  }
  for (int i = 1; i < 7; i++){
    Dynamixel.ledState(i, 1);
  }
  for (int i = 1; i < 7; i++){
    Dynamixel.servo(i, 512, 0x050); // id, pos, speed
  }
  delay(3000);
  for (int i = 1; i < 7; i++){
    Dynamixel.ledState(i, 0);
  }
}

void loop() {
    Dynamixel.servo(2, 700, 0x030);
    Dynamixel.servo(4, 700, 0x030);
    Dynamixel.servo(6, 700, 0x030);
  delay(2000);
    Dynamixel.servo(1, 700, 0x050);
  delay(2000);
    Dynamixel.servo(1, 512, 0x050);
  delay(2000);
    Dynamixel.servo(1, 300, 0x050);
  delay(2000);
    Dynamixel.servo(1, 512, 0x050);
  delay(2000);
    
    Dynamixel.servo(2, 512, 0x030);
    Dynamixel.servo(4, 512, 0x030);
    Dynamixel.servo(6, 512, 0x030);
  delay(2000);
    Dynamixel.servo(2, 350, 0x030);
    Dynamixel.servo(4, 350, 0x030);
    Dynamixel.servo(6, 350, 0x030);
  delay(2000);
    Dynamixel.servo(1, 700, 0x050);
  delay(2000);
    Dynamixel.servo(1, 512, 0x050);
  delay(2000);
    Dynamixel.servo(1, 300, 0x050);
  delay(2000);
    Dynamixel.servo(1, 512, 0x050);
  delay(2000);
    Dynamixel.servo(2, 512, 0x030);
    Dynamixel.servo(4, 512, 0x030);
    Dynamixel.servo(6, 512, 0x030);
  delay(2000);
  
}
