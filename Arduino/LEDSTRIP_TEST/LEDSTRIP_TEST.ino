#include <FastLED.h>
#define LEDS_COUNT 256
#define PIN_LEDSTRIP 2
CRGBArray<LEDS_COUNT> leds;

void setup() {
  FastLED.addLeds<WS2812, PIN_LEDSTRIP, GRB>(leds, LEDS_COUNT);
}

void loop() {
  
  leds[255] = 0xFF0000;
  FastLED.show();
  delay(1000);
  leds[255] = 0x00FF00;
  FastLED.show();
  delay(1000);
  leds[255] = 0x0000FF;
  FastLED.show();
  delay(1000);
  leds[255] = 0;
  FastLED.show();
  delay(1000);
  for (int i = 0; i < 256; i++) {
    leds[i] = 0xFFFFFF;
    FastLED.show();
    delay(10);
    leds[i] = 0;
    FastLED.show();
  }
  for (int i = 0; i < 256; i++) {
    leds[i] = 0xFFFFFF;
    FastLED.show();
    delay(50);
    leds[i] = 0;
    FastLED.show();
  }
  for (int i = 0; i < 256; i++) {
    leds[i] = 0xFFFFFF;
    FastLED.show();
    delay(100);
    leds[i] = 0;
    FastLED.show();
  }
  
}
