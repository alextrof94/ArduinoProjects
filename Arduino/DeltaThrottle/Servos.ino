#define HARDWARE_SERVOS_BAUDRATE 1000000    // Baud rate speed for Dynamixels
#define SETTINGS_SEND_DELAY 100 // microseconds

void ServosGetV() {
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    ServosV[i] = Dynamixel.readVoltage(i+1);  
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosGetT() {
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    ServosT[i] = Dynamixel.readTemperature(i+1);  
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosGetPositions() {
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    ServosPoses[i] = Dynamixel.readPosition(i+1);  
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosGetLoads() {
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    ServosLoads[i] = Dynamixel.readLoad(i+1); 
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosGetReady() {
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++) {
    ServosReady[i] = (abs((int)Dynamixel.readPosition(i + 1) - 510) < 10);
  }
}

void ServosGo() {
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.servo(i + 1, 510, 0x010);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosInit() {
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++) {
    ServosReady[i] = false;
  }
  while (millis() < 1000) ; // wait Dynamixels hardware start
  Dynamixel.begin(Serial1, HARDWARE_SERVOS_BAUDRATE);
  
  pinMode(PIN_SERVOS_CONTROL, OUTPUT);
  digitalWrite(PIN_SERVOS_CONTROL, 1);
  Dynamixel.setDirectionPin(PIN_SERVOS_CONTROL); 
  
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setMaxTorque(i + 1, 0x200);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setHoldingTorque(i + 1, 0x200);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setPunch(i + 1, 0x020);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setMode(i + 1, 1, 0x001, 0xFFF);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.ledState(i + 1, 0x001);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.ledState(i + 1, 0);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.servo(i + 1, 510, 0x010);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}
