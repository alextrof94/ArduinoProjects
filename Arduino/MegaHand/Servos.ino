#define HARDWARE_SERVOS_COUNT 6
#define SETTINGS_SEND_DELAY 200 // microseconds
#define SETTINGS_ACCURACITY 0x18 // units (1 unit for 0.29 degrees)
#define HARDWARE_SERVOS_BAUDRATE 1000000    // Baud rate speed for Dynamixels

#define HAND_ENABLED_POS 520

const uint8_t ServosSpeedCount = 5;
uint16_t ServosSpeeds[] = {
  0x005, 0x025, 0x050, 0x100, 0x3FF
};
const uint8_t ServosPosCount = 45;
int ServosPosesNeed[6];
uint8_t ServosLastPos = 255;
bool ServosReady[6] = {1,1,1,1,1,1};
uint32_t ServosSwitchTime[6] = {0,0,0,0,0,0};
uint16_t ServosPoses[][6] = {
  // delay, servos
  // init 0 - 1
  {525, 700, 497, 143, 500, 315},
  {524, 700, 497, 150, 500, 316},
  // ready 2
  {524, 700, 507, 100, 500, 312},
  // to get 3-4
  {270, 500, 454, 200, 479, 269},
  {270, 500, 451, 118, 497, 332},
  // afterget 5-6
  {254, 471, 451, 118, 497, 332},
  {263, 499, 454, 151, 479, 269},
  // check 7-8
  {430, 418, 225, 233, 399, 222},
  {422, 209, 202, 164, 497, 332},
  // checked 9
  {430, 418, 225, 233, 399, 222},
  // red 10-11
  {613, 500, 556, 338, 465, 203},
  {612, 346, 553, 334, 464, 246},
  // red after 12-13
  {612, 346, 553, 334, 464, 246},
  {613, 500, 556, 338, 465, 203},
  // green 14-15
  {532, 500, 537, 247, 486, 239},
  {531, 394, 537, 240, 479, 278},
  // green after 16-17
  {531, 394, 537, 240, 479, 278},
  {532, 500, 537, 247, 486, 239},
  // blue 18-19
  {457, 500, 502, 263, 498, 227},
  {457, 389, 501, 254, 495, 262},
  // blue after 20-21
  {457, 389, 501, 254, 495, 262},
  {457, 500, 502, 263, 498, 227},
  // yellow 22-23
  {395, 500, 483, 336, 513, 215},
  {396, 352, 481, 330, 508, 248},
  // yellow after 24-25
  {396, 352, 481, 330, 508, 248},
  {395, 500, 483, 336, 513, 215},
  // hello ready 26
  {512, 512, 512, 512, 256, 512},
  // hello 27-28
  {512, 512, 512, 512, 256, 700},
  {512, 512, 512, 512, 256, 300},
  // qr1 29-30
  {502, 455, 771, 173, 432, 258},
  {521, 342, 774, 181, 329, 253},
  // qr1 after 
  {514, 388, 768, 180, 378, 255},
  {664, 501, 513, 153, 520, 256},
  // qr2 
  {337, 457, 858, 126, 427, 251},
  {357, 309, 878, 91, 304, 208},
  // qr2 after 
  {352, 342, 896, 115, 342, 185},
  {589, 504, 517, 135, 482, 241},
  // bc1 
  {571, 535, 319, 136, 483, 256},
  {611, 314, 162, 108, 716, 228},
  // bc1 after 
  {621, 404, 141, 160, 607, 160},
  {424, 514, 454, 140, 510, 274},
  // bc2 
  {505, 512, 286, 177, 544, 296},
  {487, 364, 266, 176, 658, 270},
  // bc2 after 
  {491, 449, 267, 181, 600, 265},
  {280, 491, 551, 179, 500, 247}
};
const uint8_t ServosAnimCount = 24;
uint8_t ServosAnims[][3] = {
  // start, stop, spd
  {0, 1, 2},
  {2, 2, 2},
  {3, 4, 2},
  {5, 6, 2},
  {7, 8, 2},
  {9, 9, 2},
  {10, 11, 2},
  {12, 13, 2},
  {14, 15, 2},
  {16, 17, 2},
  {18, 19, 2},
  {20, 21, 2},
  {22, 23, 2},
  {24, 25, 2},
  {26, 26, 2},
  {27, 28, 4},
  {29, 30, 2},
  {31, 32, 2},
  {33, 34, 2},
  {35, 36, 2},
  {37, 38, 2},
  {39, 40, 2},
  {41, 42, 2},
  {43, 44, 2}
};

#define TIME_FOR_ERROR_OUT 3000
uint32_t handEnabledSwitchTime = 0;
bool handEnabled = false;
HandTypes handType = HTCLAW;

bool ServosHandCheckHall() {
  return (digitalRead(PIN_HAND_HALL0) || digitalRead(PIN_HAND_HALL1));
}

bool ServosAllIsReady() {
  int pos;
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++) {
    pos = (int)Dynamixel.readPosition(i + 1);
    delayMicroseconds(SETTINGS_SEND_DELAY);
    ServosReady[i] = (abs(pos - ServosPosesNeed[i]) <= SETTINGS_ACCURACITY);
  } 
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++) {
    /*if (!ServosReady[i] && millis() > ServosSwitchTime[i]) {
      PC.print(pos); PC.print(" / "); PC.print(ServosPosesNeed[i]);
    }*/
    Dynamixel.ledState(i + 1, !ServosReady[i]);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++) {
    if (!ServosReady[i]) {
      digitalWrite(PIN_LED, 0);
      return false;
    }
  }
  digitalWrite(PIN_LED, 1);
  return true;
}

void ServosSetPoses(int pos, uint8_t spd = 2) {
  /*PC.print("ServosSetPoses ");
  PC.print(millis());
  PC.print(" pos: ");
  PC.println(pos);*/
  if (pos >= ServosPosCount)
    return;
  if (spd > ServosSpeedCount)
    return;
  if (pos == -1) ; // do something service

  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++)
    ServosPosesNeed[i] = ServosPoses[pos][i];
  for (uint8_t i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    ServosSwitchTime[i] = millis() + TIME_FOR_ERROR_OUT;
    Dynamixel.servo(i + 1, ServosPoses[pos][i], ServosSpeeds[spd]);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}

void ServosAnim(uint8_t anim, int forceSpeed = -1) {  
  if (forceSpeed >= ServosSpeedCount || forceSpeed < -1)
    forceSpeed = -1;
  for (uint8_t pos = ServosAnims[anim][0]; pos <= ServosAnims[anim][1]; pos++) {
    ServosSetPoses(pos, (forceSpeed == -1) ? ServosAnims[anim][2] : forceSpeed);
    while(!ServosAllIsReady());
  }
  delay(200);
}

void ServosHandSetType(HandTypes type) {
  switch (handType) {
    case HTCLAW: 
      Dynamixel.setMode(7, 1, 0x001, 0xFFF);
      break;
    case HTDRILL:
      Dynamixel.setMode(7, 0, 0, 0);
      break;
    case HTLASERPOINTER:
      break;
  }
  delayMicroseconds(SETTINGS_SEND_DELAY);
}

bool ServosHandIsReady(){
  bool r = false;
  int pos;
  
  switch (handType) {
    case HTCLAW: 
      if (handEnabled){
        pos = (int)Dynamixel.readPosition(7);
        delayMicroseconds(SETTINGS_SEND_DELAY);
        r = (abs(pos - HAND_ENABLED_POS) <= SETTINGS_ACCURACITY);
        Dynamixel.ledState(7, !r);
        delayMicroseconds(SETTINGS_SEND_DELAY);
        /*if (!r && millis() > handEnabledSwitchTime) {
          PC.print(pos); PC.println(" / 532");
        }*/
      } else {
        pos = (int)Dynamixel.readPosition(7);
        delayMicroseconds(SETTINGS_SEND_DELAY);
        r = (abs(pos - 600) <= SETTINGS_ACCURACITY);
        Dynamixel.ledState(7, !r);
        delayMicroseconds(SETTINGS_SEND_DELAY);
        /*if (!r && millis() > handEnabledSwitchTime) {
          PC.print(pos); PC.println(" / 600");
        }*/
      }
      break;
    case HTDRILL:
      r = true;
      break;
    case HTLASERPOINTER:
      r = true;
      break;
  }
  
  return r;
}

void ServosHandSetEnable(uint8_t pos = 2, uint8_t spd = 1){
  handEnabledSwitchTime = millis() + TIME_FOR_ERROR_OUT;
  if (pos > 2)
    return;
  if (pos == 2)
    handEnabled = !handEnabled;
  handEnabled = pos;

  switch (handType) {
    case HTCLAW: 
      if (handEnabled)
        Dynamixel.servo(7, HAND_ENABLED_POS, ServosSpeeds[spd]);
      else
        Dynamixel.servo(7, 600, ServosSpeeds[spd]);
      break;
    case HTDRILL:
      if (handEnabled)
        Dynamixel.wheel(7, RIGHT, 0x3FF);
      else
        Dynamixel.wheel(7, RIGHT, 0); 
      break;
    case HTLASERPOINTER:
      break;
  }
  delayMicroseconds(SETTINGS_SEND_DELAY);
}


void ServosInit() {
  pinMode(PIN_HAND_HALL0, INPUT);
  pinMode(PIN_HAND_HALL1, INPUT);
  while (millis() < 1000) ; // wait Dynamixels hardware start
  Dynamixel.begin(Serial1, HARDWARE_SERVOS_BAUDRATE);
  pinMode(PIN_SERVOS_CONTROL, OUTPUT);
  digitalWrite(PIN_SERVOS_CONTROL, 1);
  Dynamixel.setDirectionPin(PIN_SERVOS_CONTROL);
  
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setPunch(i + 1, 0x8);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setMaxTorque(i + 1, 0xFFFF);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setHoldingTorque(i + 1, 0xFFFF);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.setMode(i + 1, 1, 0x001, 0xFFF);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.ledState(i + 1, 1);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
  ServosAnim(0);
  ServosAnim(1);
  ServosHandSetEnable(0);
  for (int i = 0; i < HARDWARE_SERVOS_COUNT; i++){
    Dynamixel.ledState(i + 1, 0);
    delayMicroseconds(SETTINGS_SEND_DELAY);
  }
}
