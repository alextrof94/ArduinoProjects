uint8_t getStartPosForMode(uint8_t note, uint8_t vel){
  switch (mode){
    case 0: 
      if (isReceiveEnabled)
        return (float)((note - keyboardMin) / keyboardSize) * LEDSTRIP_SIZE; 
      else
        return random(0, LEDSTRIP_SIZE);
      break;
    case 1: return 0; break;
    case 2: return 0; break;
    case 3: return 255; break;
    // add
    case 254: return 255; break;
    default: return random(0, LEDSTRIP_SIZE); break;
  }
}


uint32_t getFakeDelay() {
  switch (mode){
    case 3: return mode3Delay; break;
    default: return LIVELED_UPDATE_DELAY_MI; break;
  }
  return tchDelay;
}

uint8_t mi;

void liveledMode0Update() {
  for (mi = 0; mi < LIVELED_COUNT; mi++) {
    if (liveleds[mi].enabled) {
      liveleds[mi].color.r *= 0.995;
      liveleds[mi].color.g *= 0.995;
      liveleds[mi].color.b *= 0.995;
    }
    if (liveleds[mi].color.r == 0 && liveleds[mi].color.g == 0 && liveleds[mi].color.b == 0)
      liveleds[mi].enabled = 0;
  }
}

void liveledMode1Init() {
  for (mi = 0; mi < LIVELED_COUNT; mi++)
    liveleds[mi].enabled = 0;
  mode1FadeToBlackBy = sqrt(tchDelay / 1000);

  #ifdef DEBUG
    Serial.print("mode1FadeToBlackBy = ");
    Serial.println(mode1FadeToBlackBy);
    Serial.print("tchDelay = ");
    Serial.println(tchDelay);
    Serial.print("map = ");
    Serial.println(tchDelay / 1000);
  #endif
}

void liveledMode1Update(uint8_t forced) {
  if (forced) {
    for (mi = 0; mi < LIVELED_COUNT; mi++)
      if (liveleds[mi].enabled && liveleds[mi].livestage == 0) {
        liveleds[mi].pos++;
        if (liveleds[mi].pos == LEDSTRIP_SIZE)
          liveleds[mi].enabled = 0;
      }
  } else {
    for (mi = 0; mi < LIVELED_COUNT; mi++)
      if (liveleds[mi].enabled) {
        liveleds[mi].liveticks++;
        if (liveleds[mi].livestage == 0 && liveleds[mi].liveticks == mode1LifeTicksStage0) {
          liveleds[mi].livestage = 1;
          liveleds[mi].liveticks = 0;
        }
        if (liveleds[mi].livestage == 1 && liveleds[mi].liveticks == mode1LifeTicksStage1) {
          liveleds[mi].pos++;
          if (liveleds[mi].pos == LEDSTRIP_SIZE)
            liveleds[mi].enabled = 0;
          liveleds[mi].liveticks = 0;
        }
      }
  }
}


void liveledMode3Update() {
  for (mi = 0; mi < 3; mi++){
    liveleds[mi].pos += (liveleds[mi].livestage == 0) ? 1 : -1;
    if (liveleds[mi].pos == 0 || liveleds[mi].pos == LEDSTRIP_SIZE - 1)
      liveleds[mi].livestage = !liveleds[mi].livestage;
    liveleds[mi].liveticks = 0;
  }
}

void liveledModeDefaultUpdate() {
  for (mi = 0; mi < LIVELED_COUNT; mi++){
    if (liveleds[mi].enabled) {
      liveleds[mi].color.r *= 0.995;
      liveleds[mi].color.g *= 0.995;
      liveleds[mi].color.b *= 0.995;
    }
    if (liveleds[mi].color.r == 0 && liveleds[mi].color.g == 0 && liveleds[mi].color.b == 0)
      liveleds[mi].enabled = 0;
  }
}

void tryToChangeBicycleSubmode(uint8_t submode = 0) {
  if (mode == 254) {
    if (modeBicycleSubMode != submode)
      modeBicycleSubMode = submode;
    else
      modeBicycleSubMode = 0;
    liveledModeBicycleInit();
  }
}

void liveledModeBicycleInit() {
  for (mi = 0; mi < LIVELED_COUNT; mi++)
    liveleds[mi].enabled = 0;
  if (modeBicycleSubMode == 0) {
    for (mi = 0; mi < LEDSTRIP_SIZE / 4 + 1; mi++) {
      liveleds[mi].enabled = 1;
      liveleds[mi].pos = mi * 4;
      liveleds[mi].color = CRGB::Red;
      liveleds[mi].livestage = 0;
    }
  } else if (modeBicycleSubMode == 1) {
    liveleds[0].enabled = 1;
    liveleds[0].pos = 2;
    liveleds[0].color = MYCOLOR_ORANGE;
    liveleds[0].liveticks = 0;
    liveleds[0].livestage = 0;
  } else if (modeBicycleSubMode == 2) {
    liveleds[0].enabled = 1;
    liveleds[0].pos = LEDSTRIP_SIZE - 3;
    liveleds[0].color = MYCOLOR_ORANGE;
    liveleds[0].liveticks = 0;
    liveleds[0].livestage = 0;
  }
}

void liveledModeBicycleUpdate() {
  // 0 - idle: blinking red dots +-+-+-/-+-+-+
  // 1 - right turn: middle red, moving orange dots
  // 2 - left turn: middle red, moving orange dots
  if (modeBicycleSubMode == 0) {
    liveleds[0].liveticks++;
    if (liveleds[0].liveticks >= modeBicycleLifeTicksStage * 10){
      liveleds[0].liveticks = 0;
      liveleds[0].livestage = !liveleds[0].livestage;
      for (mi = 0; mi < LIVELED_COUNT; mi++)
        if (liveleds[mi].enabled)
          liveleds[mi].pos += (liveleds[0].livestage) ? 1 : -1;
    }
              
  } else if (modeBicycleSubMode == 1) {
    liveleds[0].liveticks++;
    if (liveleds[0].liveticks >= modeBicycleLifeTicksStage){
      liveleds[0].liveticks = 0;
      if (liveleds[0].livestage)
        liveleds[0].pos++;
      else
        liveleds[0].pos--;
      if (liveleds[0].pos == (LEDSTRIP_SIZE / 3) - 1 || liveleds[0].pos == 0)
        liveleds[0].livestage = !liveleds[0].livestage; 
    
    }
    for (mi = LEDSTRIP_SIZE / 3; mi < (LEDSTRIP_SIZE / 3) * 2; mi++)
      leds[mi] = CRGB::Red;   
      
  } else if (modeBicycleSubMode == 2) {
    liveleds[0].liveticks++;
    if (liveleds[0].liveticks >= modeBicycleLifeTicksStage){
      liveleds[0].liveticks = 0;
      if (liveleds[0].livestage)
        liveleds[0].pos++;  
      else
        liveleds[0].pos--;
      if (liveleds[0].pos == (LEDSTRIP_SIZE / 3) * 2 || liveleds[0].pos == LEDSTRIP_SIZE - 1)
        liveleds[0].livestage = !liveleds[0].livestage; 
    }
    for (mi = LEDSTRIP_SIZE / 3; mi < (LEDSTRIP_SIZE / 3) * 2; mi++)
      leds[mi] = CRGB::Red;
  }
}
