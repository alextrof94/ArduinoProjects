#define DEFAULT_FADE_TO_BLACK_BY 10

uint8_t ulsi;

void updateLiveleds(uint8_t forced = 0){
  switch (mode) {
    case 0:
      liveledMode0Update(); 
      break;
    case 1: 
      liveledMode1Update(forced);
      break;
    case 2: 
      liveledMode1Update(forced);
      break;
    case 3: 
      liveledMode3Update();
      break;
    // add
    case 254: 
      liveledModeBicycleUpdate();
      break;
    default:
      liveledModeDefaultUpdate(); 
      break;
  }
}

void initLiveleds(uint8_t forced = 0){
  switch (mode) {
    case 0:
      break;
    case 1: 
      liveledMode1Init();
      break;
    case 2: 
      break;
    case 3: 
      liveledMode3Init();
      break;
    // add
    case 254: 
      liveledModeBicycleInit();
      break;
    default:
      break;
  }
}

void nextMode(uint8_t forceMode = 255){
  fakeWorkTimer = micros() + tchDelay;
  if (forceMode == 255) {
    mode++;
    if (mode >= MAX_MODS)
      mode = 0;
  } else {
    mode = forceMode;
  }
  #ifdef DEBUG
    Serial.print("MODE = ");
    Serial.println(mode, DEC);
  #endif
  eepromSaveMode();
  initLiveleds();
}

void liveledBlink(CRGB c) {
  for (uint8_t mi = 0; mi < LEDSTRIP_SIZE; mi++) {
    leds[mi] = c;
  }
}

void liveledWork(){
  if (micros() > liveledTimer){
    liveledTimer = micros() + liveledDelay;
    updateLiveleds();
    updateLedstrip();
  }
}
