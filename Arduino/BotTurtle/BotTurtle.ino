#include <Dynamixel_Serial.h> 
#include <FastLED.h>

void LedsBackLights(uint8_t spd, bool side);

struct Motor {
  uint8_t id;
  uint8_t dirForward;
};
Motor motors[2];

struct IRSensor {
  uint8_t id;
  uint16_t value[7];
  uint16_t minValue[7];
  uint16_t maxValue[7];
  bool detected[7];
};
IRSensor irSensor;

#define MODE_COUNT 2
int8_t mode = 0;
int8_t modePrev = 0;

void lineFollow() {
  updateIRSensor();
  if (irSensor.detected[3])
    wheelStartEV3(50, 30);
  else
    wheelStartEV3(-50, 30);
}

void changeMode(int modeNew) {
  if (modeNew > MODE_COUNT - 2)
    return;
  modePrev = mode;
  mode = modeNew;
}

void setup() {
  DynamixelsInit();
  LedsInit();
}

void loop() {
  if (mode == 0)
    lineFollow();
  if (mode == 1) {
    if (modePrev != mode) {
      wheelStartEV3(0, 0);
      modePrev = mode;
    }
  }
}
