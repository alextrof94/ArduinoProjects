

#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 3); // RX, TX

#define AL 100

uint8_t packet[] = {0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

void sSend() {
  packet[7] = ~(packet[2] + packet[3] + packet[4] + packet[5] + packet[6]) & 0xFF;
  for (uint8_t i = 0; i < 8; i++)
    Serial1.write(packet[i]);  
}

void sLed(uint8_t id, uint8_t enable) {
  packet[2] = id;
  packet[3] = 4;
  packet[4] = 0x03;
  packet[5] = 0x19;
  packet[6] = enable;
  sSend();
}

void sPos(uint8_t id, uint16_t pos) {
  packet[2] = id;
  packet[3] = 4;
  packet[4] = 0x03;
  packet[5] = 0x1E;
  packet[6] = pos & 0xFF;
  sSend();
  packet[5] = 0x1F;
  packet[6] = (pos > 8) & 0xFF;
  sSend();
}


void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(1000);
  digitalWrite(13, 0);
  delay(1000);
  Serial1.begin(1000000);
}

void loop() {
    sLed(2, 1);
    delay(100);
    sLed(2, 0);
    delay(100);

}
