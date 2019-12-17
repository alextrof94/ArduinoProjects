#include <avr/sleep.h>
#include <EEPROM.h>

const uint8_t PIN_BTN_WAKEUP = 2;
const uint8_t PIN_LED = 13;
const uint8_t ADDR_VAR1 = 0;
uint8_t var1 = 0;
 
void wakeUpNow() {
  EEPROM.get(ADDR_VAR1, var1);
  if (var1 == 255) {
    var1 = 1;
    EEPROM.put(ADDR_VAR1, var1);
  }
  // for 100000 upwrite to 100000*8 do this:
  var1 *= 2; // 1 2 4 8 16 32 64 128 // 00000001 00000010 00000100 00001000 00010000 00100000 01000000 10000000 // en dis en dis en dis en dis
  EEPROM.put(ADDR_VAR1, var1);
  if (var1 == 1 || var1 == 4 || var1 == 16 || var1 == 64) {
    // do 1
  } else {
    // do !1
  }
}

void setup()
{
  pinMode(PIN_BTN_WAKEUP, INPUT_PULLUP);
  Serial.begin(9600);

  // FACTORY RESET
  if (!digitalRead(PIN_BTN_WAKEUP)) {
    // if button pressed on start
    digitalWrite(PIN_LED, 1);
    delay(1000);
    digitalWrite(PIN_LED, 0);
    if (!digitalRead(PIN_BTN_WAKEUP)) {
      // if button pressed more than 1 sec in start
      digitalWrite(PIN_LED, 1);
      delay(100);
      digitalWrite(PIN_LED, 0);
      delay(100);
      digitalWrite(PIN_LED, 1);
      delay(100);
      digitalWrite(PIN_LED, 0);
      delay(100);
      // rewrite var1 to 1 and put it to EEPROM
      var1 = 1;
      EEPROM.put(ADDR_VAR1, var1);
    }
  }
  
  sleepNow();
}
 
void sleepNow() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    attachInterrupt(digitalPinToInterrupt(PIN_BTN_WAKEUP), wakeUpNow, LOW); 
    sleep_mode();            
    // here the device is actually put to sleep!!
    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(PIN_BTN_WAKEUP));
}
 
void loop()
{
}
 
 
