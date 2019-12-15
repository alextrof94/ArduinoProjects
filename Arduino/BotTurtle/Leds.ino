#define PIN_LEDSTRIP 8
#define LEDSTRIP_SIZE 7
#define SETTINGS_LEDSTRIP_BACKLIGHT_L 2
#define SETTINGS_LEDSTRIP_BACKLIGHT_R 4

CRGBArray<LEDSTRIP_SIZE> leds;

void LedsBackLights(uint8_t spd = 0, bool side = 0) {
  if (spd < 0) {
    if (side == 0)
      leds[SETTINGS_LEDSTRIP_BACKLIGHT_L].r = spd;
    else
      leds[SETTINGS_LEDSTRIP_BACKLIGHT_R].r = spd;    
  } else {
    if (side == 0)
      leds[SETTINGS_LEDSTRIP_BACKLIGHT_L] = 0;
    else
      leds[SETTINGS_LEDSTRIP_BACKLIGHT_R] = 0;   
  }
  FastLED.show();
}

void LedsInit(){
  FastLED.addLeds<WS2812B, PIN_LEDSTRIP, GRB>(leds, LEDSTRIP_SIZE);
}
