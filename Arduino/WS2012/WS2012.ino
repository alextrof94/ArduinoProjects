#include "FastLED.h"

#define NUM_LEDS 1

#define DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];

uint32_t t, tLN;
uint16_t ic = 10;
uint16_t iln = 3000;
void setup() { 
  randomSeed(analogRead(A0));
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  t = millis() + ic;
  tLN = millis() + iln;
}

struct myLED {
  uint8_t r, g, b;
};
myLED l, ln;

void loop() { 
  // Turn the LED on, then pause
  if (millis() > t) {
    t = millis() + ic;
    if (l.r < ln.r)
      l.r++;
    if (l.g < ln.g)
      l.g++;
    if (l.b < ln.b)
      l.b++;
    if (l.r > ln.r)
      l.r--;
    if (l.g > ln.g)
      l.g--;
    if (l.b > ln.b)
      l.b--;
    leds[0] = CRGB(l.r, l.g, l.b);
    FastLED.show(); 
  }
  if (millis() > tLN) {
    tLN = millis() + iln;
    switch(random(0,10)){
      case 0: 
        ln.r = random(0, 255);
        ln.g = random(0, 255);
        ln.b = random(0, 255);
        break;
      case 1: 
        ln.r = 0;
        ln.g = random(0, 255);
        ln.b = random(0, 255);
        break;
      case 2: 
        ln.r = random(0, 255);
        ln.g = 0;
        ln.b = random(0, 255);
        break;
      case 3: 
        ln.r = random(0, 255);
        ln.g = random(0, 255);
        ln.b = 0;
        break;
      case 4: 
        ln.r = 0;
        ln.g = 0;
        ln.b = random(0, 255);
        break;
      case 5: 
        ln.r = random(0, 255);
        ln.g = 0;
        ln.b = 0;
        break;
      case 6: 
        ln.r = 0;
        ln.g = random(0, 255);
        ln.b = 0;
        break;
      case 7: case 8: case 9: case 10:
        ln.r = random(150, 255);
        ln.g = 0;
        ln.b = random(150, 255);
        break;
    }
  }
}
