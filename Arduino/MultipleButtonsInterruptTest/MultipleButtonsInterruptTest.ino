struct Button {
  uint8_t pin;
  bool inversed;
  bool state;
  bool stateOld;
  uint32_t timer;
}

void extint() {
  for (int i = 0; i < 3; i++) {
    if (!digitalRead(btns[i].pin) && btns[i].stateOld == false) {
      btns[i].state = true;
      btns[i].timer = millis();
    } else
    if (digitalRead(btns[i].pin) && btns[i].stateOld == true) {
      btns[i].state = false;
    }
    btns[i].stateOld = btns[i].state;
  }
}

Button btns[3];

void setup() {
  btns[0].pin = 3;
  btns[1].pin = 4;
  btns[2].pin = 5;
  btns[0].inversed = true;
  btns[1].inversed = true;
  btns[2].inversed = true;

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), extint, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool s = false;
  for (int i = 0; i < 3; i++) {
    if (btns[i].state == true)
      s = true;
  }
  
}
