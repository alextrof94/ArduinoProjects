#include <FastLED.h>
#define NUM_LEDS 288
#define DATA_PIN 3
#define HALL_PIN 2
CRGB leds[NUM_LEDS];

uint8_t h = 0;
uint8_t l = 0;
uint8_t lOld = 0;
uint16_t updatesPerSecond = 0;
uint32_t t = 0;

uint32_t oldTime = 0;
uint32_t period = 0;
void hallInterrupt() {
  h += 100;
  l = 0;
  period = millis() - oldTime;
  oldTime = millis();
}

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), hallInterrupt, RISING);
}


void loop() {
  /*updatesPerSecond++;
  if (millis() > t) {
    t += 1000;
    Serial.println(updatesPerSecond); 
    updatesPerSecond = 0;   
  }*/
  if (l < 31)
    l++;
  leds[l] = CHSV(h, 255, 255);
  delay(period / 50);
  FastLED.show();
  leds[l] = 0;
  
}
