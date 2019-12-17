#define CLK 7
#define DT 8
#define SW 9
#define PIN_DS18B20_1 10
#define PIN_DS18B20_2 11
#define PIN_HEATER 20
#define PIN_COOLER 21
#define PIN_AERATOR 22
#define PIN_COOLER_LED 23
#define PIN_HEATER_LED 24
#define PIN_NORMAL_LED 25
#define PIN_R 4
#define PIN_G 5
#define PIN_B 6
#define PIN_W 7

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW, TYPE2);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <TimeLib.h>
#include <DS1307RTC.h>
tmElements_t tm;

#include <EEPROM.h>

#include <OneWire.h>
OneWire ds1(PIN_DS18B20_1);
OneWire ds2(PIN_DS18B20_2);

#define ADDRESS_R 10
#define ADDRESS_G 20
#define ADDRESS_B 30
#define ADDRESS_W 40
#define ADDRESS_AOH 50 // AERO ON HOURS
#define ADDRESS_AOM 60
#define ADDRESS_AFH 70 // AERO OFF HOURS
#define ADDRESS_AFM 80
#define ADDRESS_AEN 90 // AERO ENABLED
#define ADDRESS_TL 100 // TEMP LOW
#define ADDRESS_TH 110 // TEMP LOW


int mode = 0;
/*
 * -3 firstStart
 * -2 rtcError
 * -1 rtcReset
 * 0 - RTCStatus
 * 1 - setup hours
 * ..
 * x - setup exit
 * 100 - work
 * 101 - workSimple
 * 110 - workGoToSetup
 * 111 - workGoToOff
 * 112 - off
 */

void getDeviceStatus(){  
  int firstStart = 0;
  EEPROM.get(0, firstStart);
  if (firstStart != 127){
    mode = -3;
    return;
  }
  if (!RTC.read(tm)){
    if (RTC.chipPresent()) {
      mode = -1;
      return;
    } else {
      mode = -2;
      return;
    }
  }
  mode = 100;
}

void modeRTCReset() {
  lcd.setCursor(0,0);
  lcd.print("The DS1307 is stopped.");
  lcd.setCursor(0,1);
  lcd.print("You need Go To SetupMode");
  delay(4000);
  mode = 1;
}

void modeRTCError() {
  lcd.setCursor(0,0);
  lcd.print("DS1307 read error!  Please check the circuitry.");
  lcd.setCursor(0,1);
  lcd.print("Going to sleep mode");
  delay(2000);
  lcd.backlight();
  while(true){
    delay(1);
  }
}

void modeFirstStart() {
  lcd.setCursor(0,0);
  lcd.print("FirstStart.");
  lcd.setCursor(0,1);
  lcd.print("You need Go To SetupMode");  
}

void displaySetupMode(){
  lcd.setCursor(0,0);
  lcd.print("SetupMode:");  
}

void display2Digits(int v){
  if (v < 10)
    lcd.print("0");
  lcd.print(v);
}

void display3Digits(int v){
  if (v < 100)
    lcd.print("0");
  if (v < 10)
    lcd.print("0");
  lcd.print(v);
}

uint32_t pressTime = 0;
bool pressed = false;
bool firstProcessing = true;

int myH = 0;
void modeSetupHours(){
  if (firstProcessing){
    myH = tm.Hour;
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myH++;
  if (enc1.isFastR()) 
    myH += 5;
  if (enc1.isLeft()) 
    myH--;
  if (enc1.isFastL()) 
    myH -= 5;
    
  if (myH > 23)
    myH = 0;
  if (myH < 0)
    myH = 23;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup HOURS:");
  lcd.setCursor(0,3);
  display2Digits(myH);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      tm.Hour = myH;
      RTC.write(tm);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myM = 0;
void modeSetupMinutes(){
  if (firstProcessing){
    myM = tm.Minute;
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myM++;
  if (enc1.isFastR()) 
    myM += 5;
  if (enc1.isLeft()) 
    myM--;
  if (enc1.isFastL()) 
    myM -= 5;
    
  if (myM > 59)
    myM = 0;
  if (myM < 0)
    myM = 59;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup MINUTES:");
  lcd.setCursor(0,3);
  display2Digits(myM);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      tm.Minute = myM;
      RTC.write(tm);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}


int myR = 0;
void modeSetupR(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_R, myR);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myR++;
  if (enc1.isFastR()) 
    myR += 5;
  if (enc1.isLeft()) 
    myR--;
  if (enc1.isFastL()) 
    myR -= 5;
    
  if (myR > 255)
    myR = 0;
  if (myR < 0)
    myR = 255;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup RED:");
  lcd.setCursor(0,3);
  display3Digits(myR);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_R, myR);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myG = 0;
void modeSetupG(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_G, myG);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myG++;
  if (enc1.isFastR()) 
    myG += 5;
  if (enc1.isLeft()) 
    myG--;
  if (enc1.isFastL()) 
    myG -= 5;
    
  if (myG > 255)
    myG = 0;
  if (myG < 0)
    myG = 255;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup GREEN:");
  lcd.setCursor(0,3);
  display3Digits(myG);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_G, myG);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myB = 0;
void modeSetupB(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_B, myB);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myB++;
  if (enc1.isFastR()) 
    myB += 5;
  if (enc1.isLeft()) 
    myB--;
  if (enc1.isFastL()) 
    myB -= 5;
    
  if (myB > 255)
    myB = 0;
  if (myB < 0)
    myB = 255;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup BLUE:");
  lcd.setCursor(0,3);
  display3Digits(myB);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_B, myB);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myW = 0;
void modeSetupW(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_W, myW);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myW++;
  if (enc1.isFastR()) 
    myW += 5;
  if (enc1.isLeft()) 
    myW--;
  if (enc1.isFastL()) 
    myW -= 5;
    
  if (myW > 255)
    myW = 0;
  if (myW < 0)
    myW = 255;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup WHITE:");
  lcd.setCursor(0,3);
  display3Digits(myW);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_W, myW);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}


int myAOH = 0;
void modeSetupAeroOnHours(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_AOH, myAOH);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myAOH++;
  if (enc1.isFastR()) 
    myAOH += 5;
  if (enc1.isLeft()) 
    myAOH--;
  if (enc1.isFastL()) 
    myAOH -= 5;
    
  if (myAOH > 23)
    myAOH = 0;
  if (myAOH < 0)
    myAOH = 23;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup AERO ON HOURS:");
  lcd.setCursor(0,3);
  display2Digits(myAOH);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_AOH, myAOH);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myAOM = 0;
void modeSetupAeroOnMinutes(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_AOM, myAOM);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myAOM++;
  if (enc1.isFastR()) 
    myAOM += 5;
  if (enc1.isLeft()) 
    myAOM--;
  if (enc1.isFastL()) 
    myAOM -= 5;
    
  if (myAOM > 59)
    myAOM = 0;
  if (myAOM < 0)
    myAOM = 59;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup AERO ON MINUTES:");
  lcd.setCursor(0,3);
  display2Digits(myAOM);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_AOM, myAOM);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myAFH = 0;
void modeSetupAeroOffHours(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_AFH, myAFH);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myAFH++;
  if (enc1.isFastR()) 
    myAFH += 5;
  if (enc1.isLeft()) 
    myAFH--;
  if (enc1.isFastL()) 
    myAFH -= 5;
    
  if (myAFH > 23)
    myAFH = 0;
  if (myAFH < 0)
    myAFH = 23;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup AERO OFF HOURS:");
  lcd.setCursor(0,3);
  display2Digits(myAFH);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_AFH, myAFH);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myAFM = 0;
void modeSetupAeroOffMinutes(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_AFM, myAFM);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myAFM++;
  if (enc1.isFastR()) 
    myAFM += 5;
  if (enc1.isLeft()) 
    myAFM--;
  if (enc1.isFastL()) 
    myAFM -= 5;
    
  if (myAFM > 59)
    myAFM = 0;
  if (myAFM < 0)
    myAFM = 59;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup AERO OFF MINUTES:");
  lcd.setCursor(0,3);
  display2Digits(myAFM);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_AFM, myAFM);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myAEN = 0;
void modeSetupAeroEnabled(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_AEN, myAEN);
    firstProcessing = false;
  }
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup AERO ENABLED:");
  lcd.setCursor(0,3);
  lcd.print(myAFM);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_AEN, myAEN);
      pressed = false;
      mode++;
      firstProcessing = true;
    } else {
      if (myAEN == 0)
        myAEN = 1;
      else
        myAEN = 0;
    }
  }
}


int myTL = 0;
void modeSetupTempLow(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_TL, myTL);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myTL++;
  if (enc1.isFastR()) 
    myTL += 5;
  if (enc1.isLeft()) 
    myTL--;
  if (enc1.isFastL()) 
    myTL -= 5;
    
  if (myTL > 99)
    myTL = 0;
  if (myTL < 0)
    myTL = 99;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup TEMP LOW:");
  lcd.setCursor(0,3);
  display2Digits(myTL);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_TL, myTL);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}

int myTH = 0;
void modeSetupTempHigh(){
  if (firstProcessing){
    EEPROM.get(ADDRESS_TH, myTH);
    firstProcessing = false;
  }
  if (enc1.isRight()) 
    myTH++;
  if (enc1.isFastR()) 
    myTH += 5;
  if (enc1.isLeft()) 
    myTH--;
  if (enc1.isFastL()) 
    myTH -= 5;
    
  if (myTH > 99)
    myTH = 0;
  if (myTH < 0)
    myTH = 9;
    
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("Setup TEMP HIGH:");
  lcd.setCursor(0,3);
  display2Digits(myTH);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      EEPROM.put(ADDRESS_TH, myTH);
    }
    pressed = false;
    mode++;
    firstProcessing = true;
  }
}


int myExit = 0;
void modeSetupExit(){
  displaySetupMode();
  lcd.setCursor(0,2);
  lcd.print("EXIT? 1-exit 0-restart setup");
  lcd.setCursor(0,3);
  lcd.print(myExit);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      if (myExit == 1) {
        EEPROM.put(0, 127);
        mode = 100;
      } else {
        mode = 1;
      }
      pressed = false;
      firstProcessing = true;
    } else {
      if (myExit == 0)
        myExit = 1;
      else
        myExit = 0;
    }
  }
}


void modeWorkStart() {
  lcd.setCursor(0,0);
  lcd.print("WorkMode:");
  delay(2000);  
  mode++;
}


float getTemp(int ds) {
  byte data[2];
  if (ds == 1){
    ds1.reset();
    data[0] = ds1.read();
    data[1] = ds1.read();
  } else {
    ds2.reset();
    data[0] = ds2.read();
    data[1] = ds2.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  return (float)raw / 16.0;
}

byte oldR, oldG, oldB;
void setColor(byte r, byte g, byte b, byte w = 0) {
  if (w > 0){
    r = w;
    g = w;
    b = w;
  }
  if (oldR != r){
    analogWrite(PIN_R, r);
    oldR = r;
  }
  if (oldG != g){
    analogWrite(PIN_G, g);
    oldG = g;
  }
  if (oldB != b){
    analogWrite(PIN_B, b);
    oldB = b;
  }
}

void setAeroEnabled(bool canEnable){
  // on     12:30   12:30   12:30   15:30
  // off    12:35   12:20   15:30   12:30
  // normal   y       n       y       n
  int normalized = 0;
  if (myAOH == myAFH) {
    if (myAOM > myAFM)
      normalized = 2;
    else if (myAOM < myAFM)
      normalized = -2;
    else
      normalized = 0;
  } else if (myAOH < myAFH)
    normalized = 1; 
  else
    normalized = -1;

  bool enable = true;
  switch(normalized){
    case 0: 
      // SAME hours and minutes
      enable = false; 
      break; 
    case 1: 
      // normalized DIFF hours
      enable = false;
      if (tm.Hour >= myAOH && tm.Minute >= myAOM)
        enable = true;
      if (tm.Hour >= myAFH && tm.Minute >= myAFM)
        enable = false;
      break; 
    case 2: 
      // normalized SAME hours DIFF minutes
      enable = false;
      if (tm.Hour == myAOH && tm.Minute >= myAOM)
        enable = true;
      if (tm.Hour == myAFH && tm.Minute >= myAFM)
        enable = false;
      break; 
    case -1: 
      // UNnormalized DIFF hours
      enable = true;
      if (tm.Hour >= myAFH && tm.Minute >= myAFM)
        enable = false;
      if (tm.Hour >= myAOH && tm.Minute >= myAOM)
        enable = true;
      break; 
    case -2: 
      // UNnormalized SAME hours DIFF minutes
      enable = true;
      if (tm.Hour == myAFH && tm.Minute >= myAFM)
        enable = false;
      if (tm.Hour == myAOH && tm.Minute >= myAOM)
        enable = true;
      break; 
  }
  digitalWrite(PIN_AERATOR, enable);
}

bool loaded = false;
bool tempStatusIsGood = true;
void modeWork() {
  if (!loaded) {
    EEPROM.get(ADDRESS_TL, myTL);
    EEPROM.get(ADDRESS_TH, myTH);    
  }
  lcd.setCursor(0,0);
  display2Digits(tm.Hour);
  lcd.print(":");
  display2Digits(tm.Minute);
  lcd.print(":");
  display2Digits(tm.Second);
  lcd.setCursor(11,0);
  display2Digits(tm.Day);
  lcd.print(":");
  display2Digits(tm.Month);
  lcd.print(":");
  display2Digits(tm.Year);
  
  lcd.setCursor(0,1);
  lcd.print("t1=");
  float temp1 = getTemp(1);
  display2Digits(floor(temp1));
  lcd.setCursor(11,1);
  lcd.print("t2=");
  float temp2 = getTemp(2);
  display2Digits(floor(temp2));
  
  if (temp1 < myTL){
    lcd.setCursor(0,1);
    lcd.print("HEATER ENABLED");
    digitalWrite(PIN_HEATER, 1);
    digitalWrite(PIN_COOLER, 0);
    digitalWrite(PIN_HEATER_LED, 1);
    digitalWrite(PIN_COOLER_LED, 0);
    digitalWrite(PIN_NORMAL_LED, 0);
    
    setColor(50, 0, 0);
    tempStatusIsGood = false;
  } else
  if (temp1 > myTH){
    lcd.setCursor(0,1);
    lcd.print("COOLER ENABLED");
    digitalWrite(PIN_HEATER, 0);
    digitalWrite(PIN_COOLER, 1);
    digitalWrite(PIN_HEATER_LED, 0);
    digitalWrite(PIN_COOLER_LED, 1);
    digitalWrite(PIN_NORMAL_LED, 0);
    setColor(0, 0, 50);
    tempStatusIsGood = false;
  } else {
    digitalWrite(PIN_HEATER, 0);
    digitalWrite(PIN_COOLER, 0);
    digitalWrite(PIN_HEATER_LED, 0);
    digitalWrite(PIN_COOLER_LED, 0);
    digitalWrite(PIN_NORMAL_LED, 1);
    tempStatusIsGood = true;
  }
  
  if (tm.Hour >= 22 || tm.Hour < 6){
    // night
    if (tempStatusIsGood)
      setColor(0, 0, 0);
    setAeroEnabled(false);
  } else if (tm.Hour >= 6 && tm.Hour < 12){
    // morning
    if (tempStatusIsGood)
      setColor(0, 0, 0, 127);
    setAeroEnabled(true);
  } else if (tm.Hour >= 12 && tm.Hour < 19){
    // day
    if (tempStatusIsGood)
      setColor(0, 0, 0, 255);
    setAeroEnabled(true);
  } else{
    // evening
    if (tempStatusIsGood)
      setColor(0, 0, 0, 127);
    setAeroEnabled(true);
  }
}

void modeWorkSimple(){
  modeWork();

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      pressed = false;
      mode = 110;
      firstProcessing = true;
    } else {
      mode = 111;
    }
  }
}

int myGoToSetup = 0;
void modeWorkGoToSetup(){
  modeWork();
  lcd.setCursor(0,2);
  lcd.print("Go To Setup? 1-setup 0-work");
  lcd.setCursor(0,3);
  lcd.print(myGoToSetup);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      if (myGoToSetup == 1) {
        mode = 1;
      } else {
        mode = 101;
      }
      pressed = false;
      firstProcessing = true;
    } else {
      if (myGoToSetup == 0)
        myGoToSetup = 1;
      else
        myGoToSetup = 0;
    }
  }
}

int myGoToOff = 0;
void modeWorkGoToOff(){
  modeWork();
  lcd.setCursor(0,2);
  lcd.print("Go To OFF? 1-OFF 0-work");
  lcd.setCursor(0,3);
  lcd.print(myGoToOff);

  if (enc1.isPress() && !pressed){
    pressTime = millis();
    pressed = true;
  } 
  if (!enc1.isPress() && pressed){
    if (millis() - pressTime > 1500){
      if (myGoToOff == 1) {
        mode = 112;
      } else {
        mode = 101;
      }
      pressed = false;
      firstProcessing = true;
    } else {
      if (myGoToOff == 0)
        myGoToOff = 1;
      else
        myGoToOff = 0;
    }
  }
}

void modeOff(){
  lcd.backlight();
  digitalWrite(PIN_AERATOR, 0);
  digitalWrite(PIN_HEATER, 0);
  digitalWrite(PIN_COOLER, 0);
  digitalWrite(PIN_HEATER_LED, 0);
  digitalWrite(PIN_COOLER_LED, 0);
  digitalWrite(PIN_NORMAL_LED, 0);
  while (true){
    delay(1);
  }
}

void setup() {
  enc1.setFastTimeout(40);
  lcd.init();
  lcd.backlight();
  setSyncProvider(RTC.get);
  pinMode(PIN_AERATOR, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_COOLER, OUTPUT);
  pinMode(PIN_HEATER_LED, OUTPUT);
  pinMode(PIN_COOLER_LED, OUTPUT);
  pinMode(PIN_NORMAL_LED, OUTPUT);
}

void loop() {
  enc1.tick();
  RTC.read(tm);
  switch(mode){
    default: getDeviceStatus(); break;
    case -1: modeRTCReset(); break;
    case -2: modeRTCError(); break;
    case -3: modeFirstStart(); break;
    case 1: modeSetupHours(); break;
    case 2: modeSetupMinutes(); break;
    case 3: modeSetupR(); break;
    case 4: modeSetupG(); break;
    case 5: modeSetupB(); break;
    case 6: modeSetupW(); break;
    case 7: modeSetupAeroOnHours(); break;
    case 8: modeSetupAeroOnMinutes(); break;
    case 9: modeSetupAeroOffHours(); break;
    case 10: modeSetupAeroOffMinutes(); break;
    case 11: modeSetupAeroEnabled(); break;
    case 12: modeSetupTempLow(); break;
    case 13: modeSetupTempHigh(); break;
    case 14: modeSetupExit(); break;
    case 100: modeWorkStart(); break;
    case 101: modeWorkSimple(); break;
    case 110: modeWorkGoToSetup(); break;
    case 111: modeWorkGoToOff(); break;
    case 112: modeOff(); break;
  }
}
