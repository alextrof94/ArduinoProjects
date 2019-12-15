#define BTN_DELAY1 500000
#define BTN_DELAY2 3000000
#define BTN_DELAY3 5000000

uint8_t btn1Blinks = 0;
uint8_t btn2Blinks = 0;
void btnWork() {
  if (!digitalRead(PIN_BTN1)){
    if (!btn1Old){
      btn1Timer = micros();
      btn1Old = 1;
      tryToChangeBicycleSubmode(1);
    }
    if (micros() - btn1Timer > BTN_DELAY1 && btn1Blinks == 0) {
        liveledBlink(0xFFFFFF);
        btn1Blinks++;
    }
    if (micros() - btn1Timer > BTN_DELAY2 && btn1Blinks == 1) {
        liveledBlink(0x00FF00);
        btn1Blinks++;
    }
  }
  if (digitalRead(PIN_BTN1) && btn1Old){
    btn1Old = 0;
    btn1Blinks = 0;
    if (micros() - btn1Timer < BTN_DELAY1){
      tchDelay -= 1000;
      nextMode(mode);
      #ifdef DEBUG
        Serial.print("tchDelay = ");
        Serial.println(tchDelay);
        Serial.print("   tchBPM = ");
        Serial.println(60000000 / tchDelay, DEC);
      #endif
      
    } else if (micros() - btn1Timer > BTN_DELAY1 && micros() - btn1Timer < BTN_DELAY2){
      nextMode();
      
    } else if (micros() - btn1Timer > BTN_DELAY2){
      nextMode(254);
    }
  }
  
  if (!digitalRead(PIN_BTN2)) {
    if (!btn2Old){
      btn2Timer = micros();
      btn2Old = 1;
      tryToChangeBicycleSubmode(2);
    }
    if (micros() - btn2Timer > BTN_DELAY1 && btn2Blinks == 0) {
        liveledBlink(0xFFFFFF);
        btn2Blinks++;
    }
    if (micros() - btn2Timer > BTN_DELAY2 && btn2Blinks == 1) {
        liveledBlink(0xFF0000);
        btn2Blinks++;
    }
  }
  if (digitalRead(PIN_BTN2) && btn2Old){
    btn2Old = 0;
    btn2Blinks = 0;
    if (micros() - btn2Timer < BTN_DELAY1){
      tchDelay += 1000;
      nextMode(mode);
      #ifdef DEBUG
        Serial.print("tchDelay = ");
        Serial.println(tchDelay);
        Serial.print("   tchBPM = ");
        Serial.println(60000000 / tchDelay, DEC);
      #endif
      
    } else if (micros() - btn2Timer > BTN_DELAY1 && micros() - btn2Timer < BTN_DELAY2){
      alwaysFullLightness = !alwaysFullLightness;
      #ifdef DEBUG
        Serial.print("alwaysFullLightness = ");
        Serial.println((alwaysFullLightness)?"YES":"NO");
      #endif
      eepromSaveAlwaysFullLightness();
      nextMode(mode);
      
    } else if (micros() - btn2Timer > BTN_DELAY2){
      if (isReceiveEnabled)
        isReceiveEnabled = 0;
      else
        isReceiveEnabled = 1;
      #ifdef DEBUG
        Serial.print("IS RECEIVE ENABLED = ");
        Serial.println((isReceiveEnabled)?"YES":"NO");
      #endif
      eepromSaveIsReceiveEnabled();
    }
  }

  if (!tchOld) {
    if (digitalRead(PIN_TCH)){
      if (tchTimingsIndex > -1) {
        tchTimings[tchTimingsIndex] = micros() - tchTimer;
      }
      tchTimer = micros();
      tchTimingsIndex++;
      if (tchTimingsIndex == TCH_SAMPLE_MAX_COUNT) {
        tchTimingsIndex = -1;
        tchDelay = 0;
        for (uint8_t smpl = 0; smpl < TCH_SAMPLE_MAX_COUNT; smpl++)
          tchDelay += tchTimings[smpl];
        tchDelay /= TCH_SAMPLE_MAX_COUNT;
        eepromSaveTchDelay();
        fakeWorkTimer = micros() + tchDelay;
        liveledBlink(0xFFFFFF);
        #ifdef DEBUG
          Serial.println();
          Serial.print("tchDelay = ");
          Serial.print(tchDelay, DEC);
          Serial.print("   tchBPM = ");
          Serial.println(60000000 / tchDelay, DEC);
          Serial.println();
          nextMode(mode);
        #endif
      }
      tchOld = 1;
    }
    if (tchTimingsIndex > -1 && micros() > tchTimer + BTN_DELAY3) {
      tchTimingsIndex = -1;
      #ifdef DEBUG
        Serial.println("TOUCH SEQUENSE RESETTED"); 
      #endif
      liveledBlink(0xFF0000);
    }
  }
  if (!digitalRead(PIN_TCH) && tchOld){
    tchOld = 0;
  }
}
