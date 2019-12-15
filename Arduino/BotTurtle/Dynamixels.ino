#define PIN_DYNAMIXEL_CONTROL 2
#define HARDWARE_DYNAMIXEL_BAUDRATE 1000000
#define SETTINGS_MAX_SPEED 0x100 // 0x3FF = MAX
#define SETTINGS_SEND_DELAY 100 // microseconds


void resetIRSensor() {
  for (uint8_t i = 0; i < 7; i++){
    irSensor.minValue[i] = 1023;
    irSensor.maxValue[i] = 0;
  }
}

void setAVGTreshold() {
  for (uint8_t i = 0; i < 7; i++) {
    Dynamixel.setRAMIRTreshold(irSensor.id, i, (irSensor.minValue[i] + irSensor.maxValue[i])/2);
    delayMicroseconds(SETTINGS_SEND_DELAY);
    Dynamixel.setEEPROMIRTreshold(irSensor.id, i, (irSensor.minValue[i] + irSensor.maxValue[i])/2);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void updateIRSensor() {
  for (uint8_t i = 0; i < 7; i++) {
    irSensor.value[i] = Dynamixel.readIRFiredata(irSensor.id, i);
    if (irSensor.value[i] < irSensor.minValue[i])
      irSensor.minValue[i] = irSensor.value[i];
    if (irSensor.value[i] > irSensor.maxValue[i])
      irSensor.maxValue[i] = irSensor.value[i];
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  byte buf = Dynamixel.readIRDetected(irSensor.id);
  delayMicroseconds(SETTINGS_SEND_DELAY);
  for (uint8_t i = 0; i < 7; i++)
    irSensor.detected[i] = (buf >> i) & 1;
}

void setMotor(int8_t spd, uint8_t mtr = 0) { 
  // Дополнительная функция, которая будет использовать две другие функции для более простого управления мотором. 
  // Если скорость положительна - вращаем мотор вперед с этой скоростью, если скорость отрицательна - вращаем мотор в обратную сторону.
  if (spd >= 0) { // Если скорость положительна
    // Едь вперед
    Dynamixel.wheel(motors[mtr].id, motors[mtr].dirForward, map(spd, 0, 100, 0, SETTINGS_MAX_SPEED));
    LedsBackLights(0, mtr);
  } else {
    spd = -spd; // переворачиваем скорость относительно 0, чтобы функция setSpeed на проверке скорости ниже 0 не обнулила нам скорость.
    // Едь назад
    Dynamixel.wheel(motors[mtr].id, !motors[mtr].dirForward, map(spd, 0, 100, 0, SETTINGS_MAX_SPEED));
    LedsBackLights(spd, mtr);
  }
}

void wheelStartEV3(int8_t rot, int8_t spd){
  /* rot = -100..100, spd = -100..100 */
  // -100, 100 = setMotor(-255,0) setMotor(255,1)
  // -50, 100 =  setMotor(0,0) setMotor(255,1)
  // 0, 100 =    setMotor(255,0) setMotor(255,1)
  // 10, 100 =   setMotor(255,0) setMotor(204,1)
  // 25, 100 =   setMotor(255,0) setMotor(127,1)
  // 50, 100 =   setMotor(255,0) setMotor(0,1)
  // 75, 100 =   setMotor(255,0) setMotor(-127,1)
  // 100, 100 =  setMotor(255,0) setMotor(-255,1)

  if (rot >= 0) {
    setMotor(spd, 0);
    if (rot <= 50)
      setMotor(((50-rot)/ 50.0) * spd, 1);
    else
      setMotor(((rot - 50)/ 50.0) * spd * -1, 1);
  } else {
    setMotor(spd, 1);
    if (rot <= 50)
      setMotor(((50-rot)/ 50.0) * spd, 0);
    else
      setMotor(((rot - 50)/ 50.0) * spd * -1, 0);    
  }
}

void wheelEV3(int8_t rot, int8_t spd, float seconds){
  wheelStartEV3(rot, spd);
  delay(seconds * 1000);
  wheelStartEV3(0, 0);
}

void DynamixelsInit(){
  while (millis() < 1000) ; // wait Dynamixels hardware start
  Dynamixel.begin(Serial, HARDWARE_DYNAMIXEL_BAUDRATE);
  pinMode(PIN_DYNAMIXEL_CONTROL, OUTPUT);
  digitalWrite(PIN_DYNAMIXEL_CONTROL, 1);
  Dynamixel.setDirectionPin(PIN_DYNAMIXEL_CONTROL); 
  Dynamixel.setMode(1, 0, 0, 0);
  delayMicroseconds(SETTINGS_SEND_DELAY);
  Dynamixel.setMode(2, 0, 0, 0);
  delayMicroseconds(SETTINGS_SEND_DELAY);
  irSensor.id = 255; //?
  resetIRSensor();
  motors[0].id = 1;
  motors[0].dirForward = RIGHT;
  motors[1].id = 2;
  motors[1].dirForward = LEFT;
}
