#define PIN_LED 23
#define PIN_BTNS_1 35
#define PIN_BTNS_2 37
#define PIN_BTNS_3 39
#define PIN_BTNS_4 41
#define PIN_BTNS_5 43
#define PIN_BTNS_6 45
#define PIN_BTNS_7 47
#define PIN_BTNS_8 49
#define PIN_WATER A0
#define PIN_SERVO 10
#define PIN_LIGHT A1
#define PIN_IR_1 24
#define PIN_IR_2 22
#define PIN_SMOKE_A A3
#define PIN_SMOKE_D 52
#define PIN_PIR 31
#define PIN_TEMP 33
#define PIN_SOUND 8
#define PIN_BTN1 3
#define PIN_BTN2 2
#define PIN_POT A2

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 
#define BTN_DELAY 10000
struct MyButton {
  uint32_t lastTime;
  bool value;
  uint32_t changed;
};
MyButton btn1, btn2;

void intbtn1(){
  if (micros() > btn1.lastTime + BTN_DELAY){
    btn1.value = 1;
    btn1.changed++;
  }
}
void intbtn2(){
  if (micros() > btn2.lastTime + BTN_DELAY){
    btn2.value = 1;
    btn2.changed++;
  }
}

void setup()
{
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("alex.trofimov@");
  lcd.setCursor(9,1);
  lcd.print("chkalovc.ru");
  delay(2000);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN1), intbtn1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN2), intbtn2, RISING);
  lcd.clear();
}

void loop()
{
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(now.day(), DEC);
  lcd.print('.');
  lcd.print(now.month(), DEC);
  lcd.print('.');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(btn1.changed, DEC);
  lcd.setCursor(3, 1);
  lcd.print(btn2.changed, DEC);


  
    
  delay(100);
}
