#define CLK 2
#define DT  3 
#define SW  4

#define ADDRESS_PH_LOW  10
#define ADDRESS_PH_HIGH 20

#define PIN_RELAY_1 10
#define PIN_RELAY_2 11
#define PIN_DS18B20 5

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); 

#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads;

#include <EEPROM.h>

#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(PIN_DS18B20);
DallasTemperature ds18b20(&oneWire);
DeviceAddress insideThermometer;
float temp = 0;
uint32_t tempUpdateTimer = 0;
const int tempUpdateDelay = 1000;

float multiplier = 0.1875f;

int mode = 0;
bool firstProcessing = true;
bool pressed = false;
uint32_t pressTime = 0;
uint32_t pressTimeNeed = 500;
uint16_t phAbsMax = 1400; // 14.00

int32_t phLow = 0;
int32_t phHigh = 0;

bool relay1 = false, relay1Old = false;
bool relay2 = false, relay2Old = false;

void displayPh(uint16_t ph){
  if ((ph / 100) < 10)
  lcd.print(" ");
  lcd.print(ph / 100);
  lcd.print(".");
  if ((ph % 100) < 10)
  lcd.print("0");
  lcd.print(ph % 100);
}

void displayMv(uint16_t mv){
  if (mv < 10000)
    lcd.print(" ");
  if (mv < 1000)
    lcd.print(" ");
  if (mv < 100)
    lcd.print(" ");
  if (mv < 10)
    lcd.print(" ");
  lcd.print(mv);
}

void modeLoading() {
  lcd.clear();
  /* FIRST START */
  uint8_t buf = 0;
  EEPROM.get(0, buf);
  if (buf != 127){
    lcd.setCursor(0,0);
    lcd.print("FIRST START");
    lcd.setCursor(0,1);
    lcd.print("GO TO SETTINGS");
    delay(2000);   
    mode = 11;
    return;
  }
  
  /* FULL RESET */
  if (enc1.isHold()){
    mode = -1;
    return;
  }

  /* WORK MODE */
  EEPROM.get(ADDRESS_PH_LOW, phLow);
  EEPROM.get(ADDRESS_PH_HIGH, phHigh);
  lcd.setCursor(0,0);
  lcd.print("WORK MODE");
  delay(2000);   
  mode = 1;
  
}

uint32_t displayUpdateTimer = 0;
uint32_t displayUpdateDelay = 500;
uint16_t phUE = 0;
uint16_t phVoltage = 0, phVoltageOld = 0;
uint16_t phNow = 0, phNowOld = 0;
void modeWork() {
  if (firstProcessing){
    lcd.clear();
    firstProcessing = false;
    lcd.setCursor(0, 0);
    lcd.print("PH MIN: ");
    displayPh(phLow);
    lcd.setCursor(0, 1);
    lcd.print("PH MAX: ");
    displayPh(phHigh);
    lcd.setCursor(0, 2);
    lcd.print("PH NOW: ");
    displayPh(phNow);
    lcd.setCursor(0, 3);
    lcd.print("TEMP: ");
    lcd.print(temp);
    lcd.setCursor(16, 0);
    lcd.print("R1 ");
    lcd.print(relay1);
    lcd.setCursor(16, 1);
    lcd.print("R2 ");
    lcd.print(relay2);
  }
  
  /* TEMP */
  if (millis() > tempUpdateTimer) {
    tempUpdateTimer = millis() + tempUpdateDelay;
    uint32_t old = millis();
    ds18b20.requestTemperatures();
    Serial.println(millis() - old);
    temp = ds18b20.getTempC(insideThermometer);
  }

  /* PH */
   phUE = ads.readADC_SingleEnded(3);
//  phUE = ads.readADC_Differential_0_1();
  phVoltage = (float)phUE * multiplier;
  phNow = phVoltage;

  float phA6 = (analogRead(A6) * 5) / 1024.0;

  if (phNow < phLow){
    relay1 = true;
    relay2 = false;
  } else
  if (phNow > phHigh){
    relay1 = false;
    relay2 = true;
  } else {
    relay1 = false;
    relay2 = false;    
  }
  digitalWrite(PIN_RELAY_1, relay1);
  digitalWrite(PIN_RELAY_2, relay2);

  if (millis() > displayUpdateTimer){
    displayUpdateTimer = millis() + displayUpdateDelay;
    lcd.setCursor(8, 2);
    lcd.print("     ");
    lcd.setCursor(8, 2);
    displayPh(phNow);
    lcd.setCursor(14, 2);
    lcd.print(phA6);
    lcd.setCursor(6, 3);
    lcd.print("     ");
    lcd.setCursor(6, 3);
    lcd.print(temp); lcd.print(" C");
    lcd.setCursor(19, 0); 
    lcd.print(relay1);
    lcd.setCursor(19, 1); 
    lcd.print(relay2);
  }
  
  if (enc1.isHold()){
    if (!pressed){
      pressed = true;
      pressTime = millis();
    }
    if (millis() - pressTime > pressTimeNeed){
      lcd.setCursor(0,3);
      lcd.print("RELEASE FOR SETTINGS");
    }
  }
  if (!enc1.isHold() && pressed) {
    pressed = false;
    if (millis() - pressTime > pressTimeNeed){
      firstProcessing = true;
      mode = 11;
    }
  }
}

void modeUnwork() {
  relay1 = false;
  relay2 = false;
  digitalWrite(PIN_RELAY_1, relay1);
  digitalWrite(PIN_RELAY_2, relay2);
}

void modeSetPhLow() {
  if (firstProcessing) {
    EEPROM.get(ADDRESS_PH_LOW, phLow);
    firstProcessing = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SET PH LOW");
  }
  
  if (enc1.isFastR())
    phLow += 10;
  if (enc1.isFastL()) 
    phLow -= 10;
  if (enc1.isRight())
    phLow += 1;
  if (enc1.isLeft())
    phLow -= 1;

  if (phLow > phAbsMax)
    phLow = phAbsMax;
  if (phLow < 0)
    phLow = 0;

  lcd.setCursor(0,1);
  displayPh(phLow);

  if (enc1.isHold()){
    if (!pressed){
      pressed = true;
      pressTime = millis();
    }
    if (millis() - pressTime > pressTimeNeed){
      lcd.setCursor(0,3);
      lcd.print("RELEASE FOR SAVE");
    }
  }
  if (!enc1.isHold() && pressed) {
    pressed = false;
    if (millis() - pressTime > pressTimeNeed){
      EEPROM.put(ADDRESS_PH_LOW, phLow);
      firstProcessing = true;
      mode++;
    }
  }
}

void modeSetPhHigh() {
  if (firstProcessing) {
    EEPROM.get(ADDRESS_PH_HIGH, phHigh);
    if (phHigh < phLow)
      phHigh = phLow;
    firstProcessing = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SET PH HIGH");
  }
  
  if (enc1.isFastR())
    phHigh += 10;
  if (enc1.isFastL()) 
    phHigh -= 10;
  if (enc1.isRight())
    phHigh += 1;
  if (enc1.isLeft())
    phHigh -= 1;

  if (phHigh > phAbsMax)
    phHigh = phAbsMax;
  if (phHigh < phLow)
    phHigh = phLow;

  lcd.setCursor(0,1);
  displayPh(phHigh);

  if (enc1.isHold()){
    if (!pressed){
      pressed = true;
      pressTime = millis();
    }
    if (millis() - pressTime > pressTimeNeed){
      lcd.setCursor(0,3);
      lcd.print("RELEASE FOR SAVE");
    }
  }
  if (!enc1.isHold() && pressed) {
    pressed = false;
    if (millis() - pressTime > pressTimeNeed){
      EEPROM.put(ADDRESS_PH_HIGH, phHigh);
      firstProcessing = true;
      mode++;
    }
  }
}

void modeSetupCompleted() {
  EEPROM.put(0, 127);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SETUP COMPLETED!");
  lcd.setCursor(0,1);
  lcd.print("GO TO WORK MODE");
  delay(2000);
  mode = 1;
}

uint8_t isHardReset = false;
void modeHardReset() {
  if (firstProcessing) {
    firstProcessing = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("YOU REALLY WANT");
    lcd.setCursor(0,1);
    lcd.print("HARD RESET?");
    delay(1000);
  }
  
  if (enc1.isRight())
    isHardReset = 1;
  if (enc1.isLeft())
    isHardReset = 0;
    
  lcd.setCursor(0,2);
  lcd.print(isHardReset);
  
  if (enc1.isHold()){
    if (!pressed){
      pressed = true;
      pressTime = millis();
    }
    if (millis() - pressTime > pressTimeNeed){
      lcd.setCursor(0,3);
      lcd.print("RELEASE FOR SAVE");
    }
  }
  if (!enc1.isHold() && pressed) {
    pressed = false;
    if (millis() - pressTime > pressTimeNeed){
      if (isHardReset){
        EEPROM.put(0, 128);
        EEPROM.put(ADDRESS_PH_LOW, 0);
        EEPROM.put(ADDRESS_PH_HIGH, 0);
      }
      firstProcessing = true;
      mode = 0;
    }
  }
}

void isrCLK() {
  enc1.tick();  // отработка в прерывании
}
void isrDT() {
  enc1.tick();  // отработка в прерывании
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  Serial.begin(57600);
  
  enc1.setType(TYPE2);
  attachInterrupt(0, isrCLK, CHANGE);    // прерывание на 2 пине! CLK у энка
  attachInterrupt(1, isrDT, CHANGE);    // прерывание на 3 пине! DT у энка

  
  ads.setGain(GAIN_TWOTHIRDS); multiplier = 0.1875f;    // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
//  ads.setGain(GAIN_ONE);       multiplier = 0.125f;     // 1x gain   +/- 4.096V  1 bit = 0.125mV
//  ads.setGain(GAIN_TWO);       multiplier = 0.0625f;    // 2x gain   +/- 2.048V  1 bit = 0.0625mV
//  ads.setGain(GAIN_FOUR);      multiplier = 0.03125f;   // 4x gain   +/- 1.024V  1 bit = 0.03125mV
//  ads.setGain(GAIN_EIGHT);     multiplier = 0.015625f;  // 8x gain   +/- 0.512V  1 bit = 0.015625mV
//  ads.setGain(GAIN_SIXTEEN);   multiplier = 0.0078125f; // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  ads.begin();

  ds18b20.begin();
  Serial.print("FOUND DS18B20: ");
  Serial.println(ds18b20.getDeviceCount(), DEC);
  if (!ds18b20.getAddress(insideThermometer, 0)) 
    Serial.println("Unable to find address for Device 0"); 
  
  ds18b20.setResolution(insideThermometer, 12);

  pinMode(A6, INPUT);
}

void loop() {
  enc1.tick();
  switch(mode) {
    case -1: modeHardReset(); break;
    case 0: modeLoading(); break;
    case 1: modeWork(); break;
    case 11: modeSetPhLow(); break;
    case 12: modeSetPhHigh(); break;
    case 13: modeSetupCompleted(); break;
    default: break;
  }
  if (mode != 1)
    modeUnwork();
}
