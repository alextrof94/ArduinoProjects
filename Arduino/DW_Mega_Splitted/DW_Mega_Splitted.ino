#include <FastLED.h>
#define NUM_LEDS 288
#define LEDS_FOR_SEGMENT 96
#define PIN_LEDSTRIP_0 23
#define PIN_LEDSTRIP_1 25
#define PIN_LEDSTRIP_2 27
const uint8_t hallCount = 1;
const uint8_t hallPins[] = {2};
CRGB leds[NUM_LEDS];

uint8_t h = 0;
uint16_t l = 0;
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
  FastLED.addLeds<WS2812B, PIN_LEDSTRIP_0>(leds, LEDS_FOR_SEGMENT * 0, LEDS_FOR_SEGMENT);
  FastLED.addLeds<WS2812B, PIN_LEDSTRIP_1>(leds, LEDS_FOR_SEGMENT * 1, LEDS_FOR_SEGMENT);
  FastLED.addLeds<WS2812B, PIN_LEDSTRIP_2>(leds, LEDS_FOR_SEGMENT * 2, LEDS_FOR_SEGMENT);
  Serial.begin(115200);
  for (int i = 0; i < hallCount; i++)
    pinMode(hallPins[i], INPUT);
  attachInterrupt(digitalPinToInterrupt(hallPins[0]), hallInterrupt, RISING);
}


void loop() {
  updatesPerSecond++;
  if (millis() > t) {
    Serial.println(updatesPerSecond); 
    updatesPerSecond = 0;   
    t = millis() + 1000;
  }
  l++;
  if (l == 288)
    l = 0;
  leds[l] = CHSV((uint8_t)l, 255, 255);
  FastLED.show();
  leds[l] = 0;
  
}
