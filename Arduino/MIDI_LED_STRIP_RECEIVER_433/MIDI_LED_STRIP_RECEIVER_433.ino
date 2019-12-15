#include "FastLED.h"
#include "EEPROM.h"
#include "VirtualWire.h"

#define DEBUG
//#define FULLDEBUG

// functions
void nextMode(uint8_t forceMode = 255);
void initLiveleds(uint8_t forced = 0);
void tryToChangeBicycleSubmode(uint8_t submode = 0);

#define MYCOLOR_ORANGE 0xFF2000

#define LEDSTRIP_SIZE 42
#define PIN_LIGHT A0
#define PIN_LIGHT_SUP_L 13
#define PIN_LIGHT_SUP_H A1
#define PIN_TCH 4
#define PIN_BTN1 2
#define PIN_BTN2 3
#define PIN_LEDSTRIP 12

#define PIN_433 11
#define CODE_PIANO 127
#define CODE_BICYCLE 128

uint8_t modeBicycleSubMode = 0;
// 0 - idle: blinking red dots +-+-+-/-+-+-+
// 1 - right turn: middle red, moving orange dots
// 2 - left turn: middle red, moving orange dots

uint8_t mhz433buf[3];

CRGBArray<LEDSTRIP_SIZE> leds;
uint8_t lightness = 255;
uint8_t lightnessOld = 255;

bool isReceiveEnabled = 0;

#define MAX_MODS 5 // + 1 def
uint8_t mode = 0;
/*
0 - pos of note, rb color of note, fade off
1 - running light RB
2 - running light RND
3 - 3 rinning dots
def - random pos, random color, fade off
*/
uint16_t mode1LifeTicksStage0 = 30;
uint16_t mode1LifeTicksStage1 = 10;
uint16_t mode1FadeToBlackBy = 30;
uint16_t mode3Delay = 100000;
uint16_t mode3FadeToBlackBy = 200;

uint16_t modeBicycleLifeTicksStage = 3;

struct Liveled {
  uint16_t liveticks = 0;
  uint16_t livestage = 0;
  uint8_t note = 0;
  uint8_t vel = 0;
  uint8_t pressed = 0;
  uint8_t enabled = 0;
  uint8_t pos = 0;
  CRGB color = 0;
};

#define LIVELED_COUNT 50 // LESS THAN 254
Liveled liveleds[LIVELED_COUNT];
#define LIVELED_UPDATE_DELAY_MI 100
uint32_t liveledTimer = 0;

bool btn1Old = 0;
uint32_t btn1Timer = 0;
bool btn2Old = 0;
uint32_t btn2Timer = 0;

#define TCH_SAMPLE_MAX_COUNT 7
uint32_t tchTimings[TCH_SAMPLE_MAX_COUNT];
int tchTimingsIndex = -1;
bool tchOld = 0;
uint32_t tchTimer = 0;
uint32_t tchDelay = 1000;
uint32_t fakeWorkTimer = 0;
uint32_t liveledDelay = 0;
uint8_t alwaysFullLightness = 0;

uint8_t keyboardMin = 255;
uint8_t keyboardMax = 0;
uint8_t keyboardSize = 0;

void setup() {
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(8000);   // Bits per sec
  vw_rx_start();
  #ifdef DEBUG
    Serial.begin(56700);
  #endif
  
  randomSeed(analogRead(A0));
  FastLED.addLeds<WS2812, PIN_LEDSTRIP, GRB>(leds, LEDSTRIP_SIZE);
  Serial.println("MIDI LEDSTRIP RECEIVER 433 v1.0");

  pinMode(PIN_TCH, INPUT);
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_LIGHT_SUP_H, OUTPUT);
  pinMode(PIN_LIGHT_SUP_L, OUTPUT);
  digitalWrite(PIN_LIGHT_SUP_H, 1);
  digitalWrite(PIN_LIGHT_SUP_L, 0);
  
  eepromLoad();
}

void loop() {
  btnWork();
  if (isReceiveEnabled) {
    mhz433Work();
    liveledDelay = LIVELED_UPDATE_DELAY_MI;
  } else {
    if (mode != 254 && mode != 3) {
      // fake work
      if (micros() >= fakeWorkTimer) {
        if (alwaysFullLightness)
          lightness = 255;
        else
          switch (analogRead(A0) / 128){
            case 0: lightness = 5; break;
            case 1: lightness = 10; break;
            case 2: lightness = 15; break;
            case 3: lightness = 20; break;
            case 4: lightness = 40; break;
            case 5: lightness = 70; break;
            case 6: lightness = 150; break;
            case 7: lightness = 255; break;
          }
          if (lightnessOld != lightness) {
            #ifdef DEBUG
              Serial.print("Lightness = ");
              Serial.println(lightness);
              lightnessOld = lightness;
            #endif
          }
        mhz433buf[1] = random(1, 255);
        mhz433buf[2] = lightness;
        pianoWork();
        fakeWorkTimer = micros() + tchDelay;
      }
    }
    liveledDelay = getFakeDelay();
  }
  liveledWork();
}
