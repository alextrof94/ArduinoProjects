uint8_t found;
uint8_t mhz433i;

void pianoWork() {
  found = 255;
  for (mhz433i = 0; mhz433i < LIVELED_COUNT; mhz433i++)
    if (liveleds[mhz433i].enabled == 1 && liveleds[mhz433i].note == mhz433buf[1] && liveleds[mhz433i].pressed == 1){
      found = mhz433i;
      break;
    }
  if (found < 255){
    // found pressed liveLed
    liveleds[found].vel = mhz433buf[2];
    if (mhz433buf[2] > 0) {
      // if re-pressed (if was bug for un-pressing)
      liveleds[found].pos = getStartPosForMode(liveleds[found].note, liveleds[found].vel);
      liveleds[found].color = getStartColorForMode(liveleds[found].note, liveleds[found].vel);
      liveleds[found].liveticks = 0;
      liveleds[found].livestage = 0;
      liveleds[found].pressed = 1;
    } else {
      // un-pressed
      liveleds[found].pressed = 0;
    }
  } else {
    // is new liveLed
    updateLiveleds(1); // updateForced
    for (mhz433i = 0; mhz433i < LIVELED_COUNT; mhz433i++)
      if (liveleds[mhz433i].enabled == 0){
        // found free liveLed - set it
        liveleds[mhz433i].enabled = 1;
        liveleds[mhz433i].note = mhz433buf[1];
        liveleds[mhz433i].vel = mhz433buf[2];
        liveleds[mhz433i].pos = getStartPosForMode(liveleds[mhz433i].note, liveleds[mhz433i].vel);
        liveleds[mhz433i].color = getStartColorForMode(liveleds[mhz433i].note, liveleds[mhz433i].vel);
        liveleds[mhz433i].pressed = 1;
        liveleds[mhz433i].liveticks = 0;
        liveleds[mhz433i].livestage = 0;
        break;
      }
  }
}

void mhz433Work() {
  if (vw_get_message(mhz433buf, 3)) {
    // got message
    if (mhz433buf[0] == CODE_PIANO){
      if (mhz433buf[1] < keyboardMin) {
        keyboardMin = mhz433buf[1];
        keyboardSize = keyboardMax - keyboardMin;
        eepromSaveKeyboardMin();
      }
      if (mhz433buf[1] > keyboardMax) {
        keyboardMax = mhz433buf[1];
        keyboardSize = keyboardMax - keyboardMin;
        eepromSaveKeyboardMax();
      }
      pianoWork();
    } else
    if (mhz433buf[0] == CODE_BICYCLE){
      mode = 255;
      modeBicycleSubMode = mhz433buf[1];
      liveledModeBicycleInit();
    }
  }
}
