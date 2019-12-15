#define EEPROM_INITIALIZED 0
#define EEPROM_KEYBOARD_MIN 10
#define EEPROM_KEYBOARD_MAX 11
#define EEPROM_IS_RECEIVE_ENABLED 12
#define EEPROM_MODE 13
#define EEPROM_TCH_DELAY 14 // - 17
#define EEPROM_ALWAYS_FULL_LIGHTNESS 18

uint8_t eepromInitialized = 0;
void eepromLoad() {
  EEPROM.get(EEPROM_INITIALIZED, eepromInitialized);
  if (eepromInitialized == 255){
    EEPROM.put(EEPROM_KEYBOARD_MIN, 255);
    EEPROM.put(EEPROM_KEYBOARD_MAX, 0);
    EEPROM.put(EEPROM_IS_RECEIVE_ENABLED, 0);
    EEPROM.put(EEPROM_MODE, 0);
    EEPROM.put(EEPROM_TCH_DELAY, 1000);
    EEPROM.put(EEPROM_ALWAYS_FULL_LIGHTNESS, 0);
    EEPROM.put(EEPROM_INITIALIZED, 0);
  }
  
  EEPROM.get(EEPROM_KEYBOARD_MIN, keyboardMin);
  EEPROM.get(EEPROM_KEYBOARD_MAX, keyboardMax);
  EEPROM.get(EEPROM_IS_RECEIVE_ENABLED, isReceiveEnabled);
  EEPROM.get(EEPROM_MODE, mode);
  EEPROM.get(EEPROM_TCH_DELAY, tchDelay);
  EEPROM.get(EEPROM_ALWAYS_FULL_LIGHTNESS, alwaysFullLightness);
  tchDelay = 1000000;
  
  #ifdef DEBUG
    Serial.println("EEPROM:");
    Serial.print("KEYBOARD MIN = ");
    Serial.println(keyboardMin);
    Serial.print("KEYBOARD MAX = ");
    Serial.println(keyboardMax);
    Serial.print("IS RECEIVE ENABLED = ");
    Serial.println((isReceiveEnabled)?"YES":"NO");
    Serial.print("MODE = ");
    Serial.println(mode);
    Serial.print("tchDelay = ");
    Serial.println(tchDelay);
    Serial.print("alwaysFullLightness = ");
    Serial.println(alwaysFullLightness);
    Serial.println();
  #endif
  
  keyboardSize = keyboardMax - keyboardMin;
  nextMode(mode);
}

void eepromSaveKeyboardMin() { EEPROM.put(EEPROM_KEYBOARD_MIN, keyboardMin); }
void eepromSaveKeyboardMax() { EEPROM.put(EEPROM_KEYBOARD_MAX, keyboardMax); }
void eepromSaveIsReceiveEnabled() { EEPROM.put(EEPROM_IS_RECEIVE_ENABLED, isReceiveEnabled); }
void eepromSaveMode() { EEPROM.put(EEPROM_MODE, mode); }
void eepromSaveTchDelay() { EEPROM.put(EEPROM_TCH_DELAY, tchDelay); }
void eepromSaveAlwaysFullLightness() { EEPROM.put(EEPROM_ALWAYS_FULL_LIGHTNESS, alwaysFullLightness); }
