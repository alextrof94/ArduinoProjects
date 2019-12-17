#include "FastLED.h"
#define NUM_LEDS 1
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

#define BUTTON_DELAY 1000
#define COLOR_CHANGE_DELAY 5000

#include <EEPROM.h>

uint8_t r, g, b, t;
uint32_t timerB;
uint8_t btn;

uint32_t change_pause = 1;
#define PAUSE_CHANGES 7
uint8_t change_pause_count = 0;

uint8_t mode = 0;
#define MODES 2

uint8_t rn, gn, bn;
uint32_t timerC;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  Serial.begin(9600);
  EEPROM.get(0, mode);
  EEPROM.get(10, change_pause);
  EEPROM.get(20, change_pause_count);
  if (mode > MODES){
    mode = 0;
    change_pause = 1;
    change_pause_count = 0;
  }
  Serial.print("mode=");
  Serial.println(mode);
  Serial.print("change_pause=");
  Serial.println(change_pause);
  Serial.print("change_pause_count=");
  Serial.println(change_pause_count);
}

void myBlink(){
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
  delay(100);
  leds[0] = CRGB(255, 255, 255);
  FastLED.show();
  delay(100);  
}

void loop() {
  if (digitalRead(3) && !btn){
    timerB = millis();
    btn = 1;
  }
  if (!digitalRead(3) && btn){
    btn = 0;
    if (millis() - timerB <= BUTTON_DELAY){
      change_pause *= 2;
      change_pause_count++;
      if (change_pause_count == PAUSE_CHANGES) {
        myBlink();
        myBlink();
      }
      if (change_pause_count > PAUSE_CHANGES) {
        change_pause = 1;
        change_pause_count = 0;
        myBlink();
        myBlink();
        myBlink();
        myBlink();
      }
      myBlink();
      Serial.print("change_pause=");
      Serial.println(change_pause);
      EEPROM.put(10, change_pause);
      EEPROM.put(20, change_pause_count);
    } else    
    if (millis() - timerB > BUTTON_DELAY){
      mode++;
      if (mode == MODES - 1) {
        myBlink();
        myBlink();
      }
      if (mode > MODES - 1) {
        mode = 0;
        myBlink();
        myBlink();
        myBlink();
        myBlink();
      }
      myBlink();
      Serial.print("mode=");
      Serial.println(mode);
      EEPROM.put(0, mode);
    }
  }
  if (mode == 0) {
    if (t == 0){
      g = 255;
      r++;
      if (r == 255){
        t++;
        delay(change_pause*10);
      }
    }
    if (t == 1){
      r = 255;
      g--;
      if (g == 0){
        t++;
        delay(change_pause*10);
      }
    }
    if (t == 2){
      r = 255;
      b++;
      if (b == 255){
        t++;
        delay(change_pause*10);
      }
    }
    if (t == 3){
      b = 255;
      r--;
      if (r == 0){
        t++;
        delay(change_pause*10);
      }
    }
    if (t == 4){
      b = 255;
      g++;
      if (g == 255){
        t++;
        delay(change_pause*10);
      }
    }
    if (t == 5){
      g = 255;
      b--;
      if (b == 0){
        t++;
        delay(change_pause*10);
      }
    }
    if (t > 5)
      t = 0;
    delay(change_pause);
  }
  if (mode == 1){
    if (millis() > timerC){
      timerC = millis() + COLOR_CHANGE_DELAY;
      rn = random(30, 255);
      gn = random(30, 255);
      bn = random(0, 255);
    }
    if (rn > r) r++;
    if (rn < r) r--;
    if (gn > g) g++;
    if (gn < g) g--;
    if (bn > b) b++;
    if (bn < b) b--;     
    delay(change_pause*10); 
  }
  leds[0] = CRGB(r,g,b);
  FastLED.show();
}



































