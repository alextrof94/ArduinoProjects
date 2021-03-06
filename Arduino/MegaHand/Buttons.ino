const uint8_t btnsCount = 1;
Button btns[btnsCount];

// Interrupt by timer 
SIGNAL(TIMER0_COMPA_vect) {
  for (uint8_t i = 0; i < btnsCount; i++) {
    btns[i].isPressed = digitalRead(btns[i].pin);
    if (btns[i].inversed)
      btns[i].isPressed = !btns[i].isPressed;
      
    if (btns[i].isPressed && !btns[i].isPressedOld) 
      btns[i].pressTime = millis();
      btns[i].releaseProcessed = false;
    } else
    if (!btns[i].isPressed && btns[i].isPressedOld) {
      btns[i].releaseTime = millis();
      btns[i].pressProcessed = false;
    }
    btns[i].isPressedOld = btns[i].isPressed;
  }
  
  for (uint8_t i = 0; i < btnsCount; i++) {
    if (!btns[i].pressProcessed){
      if (btns[i].releaseTime - btns[i].pressTime > 1000)
        mode = 2;
      else
        mode = (mode == 0) ? 1 : 0;
    }
  }
}

void ButtonsInit() { 
  btns[0].pin = A15;
  btns[0].inversed = true;
  btns[0].pullup = true;
  for (uint8_t i = 0; i < btnsCount; i++)
    pinMode(btns[i].pin, (btns[i].pullup) ? INPUT_PULLUP : INPUT);
  // set interrupt by timer every 1ms
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}
