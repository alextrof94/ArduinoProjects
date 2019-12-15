#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo
#include <Wire.h> // I2C library
#include <LiquidCrystal_I2C.h> // I2C lcd library

LiquidCrystal_I2C lcd(0x3F,20,4); // I2C lcd class 

#define PIN_SERVOS_CONTROL 4
#define PIN_LED 13
#define PIN_HAND_HALL0 40
#define PIN_HAND_HALL1 41
#define PC Serial

struct Color {
  byte r, g, b;
};

enum SimpleColors {
  SCRED, SCGREEN, SCBLUE, SCYELLOW, SCCYAN, SCPURPLE, SCWHITE, SCBLACK, SCNONE
};

enum HandTypes {
  HTCLAW, HTDRILL, HTLASERPOINTER
};

enum Anims {
  ANIMINIT, ANIMREADY, 
  ANIMTOGET, ANIMAFTERGET, 
  ANIMCHECK, ANIMCHECKED, 
  ANIMTORED, ANIMAFTERRED, 
  ANIMTOGREEN, ANIMAFTERGREEN,
  ANIMTOBLUE, ANIMAFTERBLUE,
  ANIMTOYELLOW, ANIMAFTERYELLOW,
  ANIMHELLOREADY, ANIMHELLO,
  ANIMTOQR1, ANIMAFTERQR1, 
  ANIMTOQR2, ANIMAFTERQR2,
  ANIMTOBC1, ANIMAFTERBC1, 
  ANIMTOBC2, ANIMAFTERBC2
};

const uint8_t externalTypesCount = 2;
enum ExternalTypes {
  ETNONE, ETDISPENSER
};

struct Button {
  uint8_t pin = 2;
  bool inversed = true;
  bool pullup = true;
  bool isPressed = false;
  bool isPressedOld = false;
  uint32_t pressTime;
  uint32_t releaseTime;
  bool pressProcessed = false;
  bool releaseProcessed = false;
};

uint8_t mode = 0;
uint8_t modeOld = 0;
bool modeFirstStart = true;

void ServosSetPoses(int pos, uint8_t spd = 2);
void ServosHandSetEnable(uint8_t pos = 2, uint8_t spd = 1);
void ServosAnim(uint8_t anim, int forceSpeed = -1);


String msg = "c01234567890\r\n";

void modeReady() {
    ServosAnim(ANIMREADY);
    while(!ServosAllIsReady()) ;
    delay(1000);
}

void modeWork() {
    while(!ServosAllIsReady());
    ServosHandSetEnable(0);
    while(!ServosHandIsReady());
    ServosAnim(ANIMTOGET);
    while(!ServosAllIsReady());
    ServosHandSetEnable(1);
    while(!ServosHandIsReady());
    ServosAnim(ANIMAFTERGET);
    while(!ServosAllIsReady());
    ServosAnim(ANIMCHECK);
    while(!ServosAllIsReady());
    PC.print("recognize\r\n");
    while(PC.available() < 14)
      delay(1);
    msg = PC.readString();
    ServosAnim(ANIMCHECKED);
    while(!ServosAllIsReady());
    if (msg[1] == '1') {
      // barcode
      PC.println("barcode");
      if (msg[2] == '1'){
        PC.println("barcode1");
        ServosAnim(ANIMTOBC1);
      }
      else {
        PC.println("barcode2");
        ServosAnim(ANIMTOBC2);
      }
    } else if (msg[1] == '2') {
      // qr
      if (msg[2] == 'q')
        ServosAnim(ANIMTOQR1);
      else
        ServosAnim(ANIMTOQR2);
    } else {
      switch(msg[0]) {
        case 'r': ServosAnim(ANIMTORED); break;
        case 'g': ServosAnim(ANIMTOGREEN); break;
        case 'b': ServosAnim(ANIMTOBLUE); break;
        case 'y': ServosAnim(ANIMTOYELLOW); break;
        default: ServosAnim(ANIMREADY); break;
      }
    }
    while(!ServosAllIsReady());
    ServosHandSetEnable(0);
    while(!ServosHandIsReady());
    if (msg[1] == '1') {
      // barcode 
      if (msg[2] == '9')
        ServosAnim(ANIMAFTERBC1);
      else
        ServosAnim(ANIMAFTERBC2);
    } else if (msg[1] == '2') {
      // qr
      if (msg[2] == 'q')
        ServosAnim(ANIMAFTERQR1);
      else
        ServosAnim(ANIMAFTERQR2);
    } else {
      switch(msg[0]) {
        case 'r': ServosAnim(ANIMAFTERRED); break;
        case 'g': ServosAnim(ANIMAFTERGREEN); break;
        case 'b': ServosAnim(ANIMAFTERBLUE); break;
        case 'y': ServosAnim(ANIMAFTERYELLOW); break;
        default: ServosAnim(ANIMREADY); break;
      }
    }
    while(!ServosAllIsReady());
    /* OLD COLOR DETECTION
    Color c = ColorSensorReadColor();
    ColorSensorPrintColor(c);
    SimpleColors sc = getSimpleColor(c);
    switch (sc) {
      case SCYELLOW: ServosAnim(ANIMTOYELLOW); break;
      case SCRED: ServosAnim(ANIMTORED); break;
      case SCGREEN: ServosAnim(ANIMTOGREEN); break;
      case SCBLUE: ServosAnim(ANIMTOBLUE); break;
      case SCNONE:
      default: ServosAnim(ANIMREADY); break;
    }
    while(!ServosAllIsReady());
    ServosHandSetEnable(0);
    while(!ServosHandIsReady());
    switch (sc) {
      case SCYELLOW: ServosAnim(ANIMAFTERYELLOW); break;
      case SCRED: ServosAnim(ANIMAFTERRED); break;
      case SCGREEN: ServosAnim(ANIMAFTERGREEN); break;
      case SCBLUE: ServosAnim(ANIMAFTERBLUE); break;
    }
    /* */
}

void modeManual() {
  
}

void modeHello() {
  if (modeFirstStart) {
    modeFirstStart = false;
    ServosAnim(ANIMHELLOREADY);
    while(!ServosAllIsReady());
  }
  
  ServosAnim(ANIMHELLO);
  while(!ServosAllIsReady());
}

void setup() {
  PC.begin(57600);
  LcdInit();
  ExternalInit();
  //ColorSensorInit();
  ServosInit();
  ButtonsInit();
    ServosAnim(ANIMREADY);
  pinMode(PIN_LED, OUTPUT);
}

uint8_t nextAnim = ANIMREADY;
void loop() {
  if (mode != modeOld){
    modeFirstStart = true;
    PC.print("Mode = ");
    PC.println(mode);
  }
  
  switch (mode) {
    default: modeReady(); break;
    case 1: modeWork(); break;
    case 2: modeManual(); break;
    case 3: modeHello(); break;
  }
}
