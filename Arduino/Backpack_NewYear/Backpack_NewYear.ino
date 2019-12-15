#include <FastLED.h>
#define LEDS_COUNT 256
#define PIN_LEDSTRIP 2
#define PIN_LEDSTRIP_L 3
#define PIN_LEDSTRIP_R 4
CRGBArray<LEDS_COUNT> leds;

uint8_t ledstrip = 0;

#define TEXT_CYCLE_COUNT 4
int textCycleNow = 0;
bool textStage = true;
bool wait = false;
bool wait2 = false;
bool wait3 = false;

//Б Г Д Ж З И Й Л П Ф Ц Ч Ш Щ Ь Ы Ъ Э Ю Я
//q u l w p b d k g a y x i o m s e r t z

PROGMEM const char str[]  = "   POqOTOTEXHbKA 6-9  10-13  14-17 kET ";
PROGMEM const char str2[] = "   yEHTP xKAkOBA    lEKAqPbCTOB 89   CHKALOVC.RU ";
PROGMEM const char str3[] = "   562-14-83    562-14-83 ";

#define TIMERANIM_MOVE 100
uint32_t timeranim = 0;
uint8_t animPos = 0;
#define WHT 255
#define GRY 1
#define BLE 2
#define DRK 3
#define YEL 4
const uint8_t plane1[128] PROGMEM = {
  0,0,0,0,WHT,0,0,0,
  0,0,0,WHT,WHT,WHT,0,0,
  0,0,0,WHT,WHT,WHT,0,0,
  0,0,BLE,WHT,GRY,WHT,0,0,
  0,0,BLE,WHT,GRY,WHT,0,0,
  0,0,BLE,WHT,GRY,GRY,0,0,
  0,0,BLE,WHT,GRY,GRY,0,0,
  0,0,0,WHT,GRY,GRY,GRY,0,
  0,0,0,WHT,GRY,GRY,GRY,0,
  0,0,0,WHT,WHT,GRY,GRY,0,
  0,0,0,WHT,WHT,WHT,GRY,0,
  0,0,0,WHT,WHT,WHT,0,0,
  0,0,WHT,WHT,GRY,WHT,0,0,
  0,WHT,WHT,WHT,GRY,GRY,0,0,
  WHT,WHT,WHT,WHT,GRY,GRY,0,0,
  0,0,0,0,WHT,0,0,0,
};
const uint8_t plane[128] PROGMEM = {
  0,0,0,0,GRY,0,0,0,
  0,0,0,GRY,GRY,GRY,0,0,
  0,0,0,GRY,GRY,GRY,0,0,
  0,0,BLE,GRY,WHT,GRY,0,0,
  0,0,BLE,GRY,WHT,GRY,0,0,
  0,0,BLE,GRY,WHT,WHT,0,0,
  0,0,BLE,GRY,WHT,WHT,0,0,
  0,0,0,GRY,WHT,WHT,WHT,0,
  0,0,0,GRY,WHT,WHT,WHT,0,
  0,0,0,GRY,GRY,WHT,WHT,0,
  0,0,0,GRY,GRY,GRY,WHT,0,
  0,0,0,GRY,GRY,GRY,0,0,
  0,0,GRY,GRY,WHT,GRY,0,0,
  0,GRY,GRY,GRY,WHT,WHT,0,0,
  GRY,GRY,GRY,GRY,WHT,WHT,0,0,
  0,0,0,0,GRY,0,0,0,
};
const uint8_t smoke[8] PROGMEM = {
  0,0,DRK,DRK,DRK,0,0,0
};
const uint8_t prop[16] PROGMEM = {
  0,YEL,YEL,YEL,YEL,YEL,0,0,
  0,0,0,YEL,0,0,0,0
};

#define BRIGHTNESS 80

#define TIMERTEXT_MOVE 37
#define TIMERTEXT_MOVE2 40
#define TIMERTEXT_MOVE3 40
#define STRING_WIDTH 8
#define CHAR_SPACE 1
uint32_t timertext = 0;
uint32_t timertext2 = 0;
uint32_t timertext3 = 0;
uint32_t strPos = 0; // now char from string "С Н[О]ВЫМ ГОДОМ!"
uint32_t strCharPos = 0; // now pos in char [O]
uint32_t strLastCharIndex = 0; // [O] is indexed from alphabet like
uint32_t strPos2 = 0; // now char from string "С Н[О]ВЫМ ГОДОМ!"
uint32_t strCharPos2 = 0; // now pos in char [O]
uint32_t strLastCharIndex2 = 0; // [O] is indexed from alphabet like
uint32_t strPos3 = 0; // now char from string "С Н[О]ВЫМ ГОДОМ!"
uint32_t strCharPos3 = 0; // now pos in char [O]
uint32_t strLastCharIndex3 = 0; // [O] is indexed from alphabet like
CRGB textColor = 0xFFFFFF;
CRGB textColor2 = 0xFF00FF;
CRGB textColor3 = 0xFFFF00;

#define ALPHABET_COUNT 69

const char alphabet[ALPHABET_COUNT] PROGMEM = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',  
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
 //Б    Г    Д    Ж    З    И    Й    Л    П    Ф    Ц    Ч    Ш    Щ    Ь    Ы    Ъ    Э    Ю    Я
  'q', 'u', 'l', 'w', 'p', 'b', 'd', 'k', 'g', 'a', 'y', 'x', 'i', 'o', 'm', 's', 'e', 'r', 't', 'z', 
  '.', ',', '!', '?', '<', '>', '+', '-', '=', '(', ')', ':', ' '
};

const uint8_t alphabetLengths[ALPHABET_COUNT] PROGMEM = {
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7,
  5, 5, 5, 6, 5, 5, 5, 5, 5, 7, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
  5, 5, 7, 7, 5, 5, 5, 5, 5, 7, 6, 5, 7, 8, 5, 7, 7, 5, 7, 5,
  1, 1, 1, 5, 3, 3, 3, 3, 3, 2, 2, 1, 3
};

const uint8_t chars[483] PROGMEM = {
  B00001110,
  B00010001,
  B00011111,
  B00010001,
  B00010001,
  B00010001,
  B00000000,
  
  B00001111,
  B00010001,
  B00011111,
  B00010001,
  B00010001,
  B00001111,
  B00000000,
  
  B00001110,
  B00010001,
  B00000001,
  B00000001,
  B00010001,
  B00001110,
  B00000000,
  
  B00001111,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00001111,
  B00000000,
  
  B00011111,
  B00000001,
  B00011111,
  B00000001,
  B00000001,
  B00011111,
  B00000000,
  
  B00011111,
  B00000001,
  B00000001,
  B00011111,
  B00000001,
  B00000001,
  B00000000,
  
  B00001110,
  B00010001,
  B00000001,
  B00011101,
  B00010001,
  B00001110,
  B00000000,
  
  B00010001,
  B00010001,
  B00011111,
  B00010001,
  B00010001,
  B00010001,
  B00000000,
  
  B00011111,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00011111,
  B00000000,
  
  B00011111,
  B00010000,
  B00010000,
  B00010000,
  B00010001,
  B00001110,
  B00000000,
  
  B00011001,
  B00000101,
  B00000011,
  B00000101,
  B00001001,
  B00010001,
  B00000000,
  
  B00000001,
  B00000001,
  B00000001,
  B00000001,
  B00010001,
  B00011111,
  B00000000,
  
  B01100011,
  B01010101,
  B01001001,
  B01000001,
  B01000001,
  B01000001,
  B00000000,
  
  B00010001,
  B00010011,
  B00010101,
  B00011001,
  B00010001,
  B00010001,
  B00000000,
  
  B00001110,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00001110,
  B00000000,
  
  B00001111,
  B00010001,
  B00010001,
  B00001111,
  B00000001,
  B00000001,
  B00000000,
 
  B00001110,
  B00010001,
  B00010001,
  B00010101,
  B00011001,
  B00011110,
  B00100000,
  
  B00001111,
  B00010001,
  B00010001,
  B00001111,
  B00001001,
  B00010001,
  B00000000,
  
  B00011110,
  B00000001,
  B00001110,
  B00010000,
  B00010000,
  B00001111,
  B00000000,
  
  B00011111,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00000000,
  
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00001110,
  B00000000,
  
  B00010001,
  B00010001,
  B00001010,
  B00001010,
  B00000100,
  B00000100,
  B00000000,
  
  B01000001,
  B01000001,
  B01000001,
  B01001001,
  B01001001,
  B00110110,
  B00000000,
  
  B00010001,
  B00001010,
  B00000100,
  B00000100,
  B00001010,
  B00010001,
  B00000000,
  
  B00010001,
  B00010001,
  B00011110,
  B00010000,
  B00010001,
  B00001110,
  B00000000,
  
  B00011111,
  B00001000,
  B00011111,
  B00000010,
  B00000001,
  B00011111,
  B00000000,
  
  // NUMS
  
  B00001110,
  B00010001,
  B00010011,
  B00010101,
  B00011001,
  B00010001,
  B00001110,
  
  B00000100,
  B00000110,
  B00000101,
  B00000100,
  B00000100,
  B00000100,
  B00011111,
  
  B00001110,
  B00010001,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
  B00011111,
  
  B00001110,
  B00010001,
  B00010000,
  B00001000,
  B00010000,
  B00010001,
  B00001110,
  
  B00011000,
  B00010100,
  B00010010,
  B00011111,
  B00010000,
  B00010000,
  B00010000,
  
  B00011111,
  B00000001,
  B00000001,
  B00001111,
  B00010000,
  B00010001,
  B00001110,
  
  B00001110,
  B00010001,
  B00000001,
  B00001111,
  B00010001,
  B00010001,
  B00001110,
  
  B00011111,
  B00010000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
  
  B00001110,
  B00010001,
  B00010001,
  B00001110,
  B00010001,
  B00010001,
  B00001110,
  
  B00001110,
  B00010001,
  B00010001,
  B00011110,
  B00001000,
  B00000100,
  B00000010,
  
  // RUS
  
  B00011111,
  B00000001,
  B00001111,
  B00010001,
  B00010001,
  B00001111,
  B00000000,
  
  B00011111,
  B00010001,
  B00000001,
  B00000001,
  B00000001,
  B00000001,
  B00000000,
  
  B00011110,
  B00100010,
  B00100010,
  B00100010,
  B00100010,
  B01111111,
  B01000001,
  
  B01001001,
  B01001001,
  B00101010,
  B00011100,
  B00101010,
  B01001001,
  B00000000,
  
  B00011111,
  B00010000,
  B00001000,
  B00010000,
  B00010000,
  B00011111,
  B00000000,
  
  B00010001,
  B00011001,
  B00010101,
  B00010011,
  B00010001,
  B00010001,
  B00000000,
  
  B00010101,
  B00011001,
  B00010101,
  B00010011,
  B00010001,
  B00010001,
  B00000000,
  
  B00011000,
  B00010100,
  B00010010,
  B00010001,
  B00010001,
  B00010001,
  B00000000,
  
  B00011111,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00000000,
  
  B00111110,
  B01001001,
  B01001001,
  B00111110,
  B00001000,
  B00001000,
  B00000000,
  
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00010001,
  B00111111,
  B00100000,
  
  B00010001,
  B00010001,
  B00010001,
  B00011110,
  B00010000,
  B00010000,
  B00000000,
  
  B01000001,
  B01000001,
  B01001001,
  B01001001,
  B01001001,
  B01111110,
  B00000000,
  
  B01000001,
  B01000001,
  B01001001,
  B01001001,
  B01001001,
  B11111110,
  B10000000,
  
  B00000001,
  B00000001,
  B00001111,
  B00010001,
  B00010001,
  B00001111,
  B00000000,
  
  B01000001,
  B01000001,
  B01001111,
  B01010001,
  B01010001,
  B01001111,
  B00000000,
  
  B00000110,
  B00000101,
  B00111100,
  B01000100,
  B01000100,
  B00111100,
  B00000000,
  
  B00001110,
  B00010001,
  B00011000,
  B00010000,
  B00010001,
  B00001110,
  B00000000,
  
  B00111001,
  B01000101,
  B01000111,
  B01000101,
  B01000101,
  B00111001,
  B00000000,
  
  B00011110,
  B00010001,
  B00011110,
  B00010010,
  B00010001,
  B00010001,
  B00000000,

  // CHARS
  
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B00000000,
  
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B00000001,
  
  B00000001,
  B00000001,
  B00000001,
  B00000001,
  B00000000,
  B00000001,
  B00000000,
  
  B00001110,
  B00010001,
  B00001000,
  B00000100,
  B00000000,
  B00000100,
  B00000000,
  
  B00000000,
  B00000100,
  B00000010,
  B00000001,
  B00000010,
  B00000100,
  B00000000,
  
  B00000000,
  B00000001,
  B00000010,
  B00000100,
  B00000010,
  B00000001,
  B00000000,
  
  B00000000,
  B00000000,
  B00000010,
  B00000111,
  B00000010,
  B00000000,
  B00000000,
  
  B00000000,
  B00000000,
  B00000000,
  B00000111,
  B00000000,
  B00000000,
  B00000000,
  
  B00000000,
  B00000000,
  B00000111,
  B00000000,
  B00000111,
  B00000000,
  B00000000,
  
  B00000010,
  B00000001,
  B00000001,
  B00000001,
  B00000001,
  B00000010,
  B00000000,
  
  B00000001,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000001,
  B00000000,
  
  B00000000,
  B00000000,
  B00000001,
  B00000000,
  B00000000,
  B00000001,
  B00000000,
  
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

uint8_t textView[8][8] = {
  {1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};
uint8_t textView2[8][8] = {
  {1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};
uint8_t textView3[8][8] = {
  {1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};

void wipe() {
  for (uint16_t i = 0; i <= 255; i++)
    leds[i] = 0;
}

void textMove() {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8 - 1; j++) {
      textView[i][j] = textView[i][j + 1];
    }
  }
  
  strCharPos++;
  if (strCharPos < pgm_read_byte(&alphabetLengths[strLastCharIndex])) {
    for (uint8_t i = 0; i < 7; i++) {
      textView[i][7] = (pgm_read_byte(&chars[strLastCharIndex * 7 + i]) >> strCharPos) & 1;
    }
  } else if (strCharPos > pgm_read_byte(&alphabetLengths[strLastCharIndex]) + CHAR_SPACE) {
    strCharPos = 0;
    strPos++;
    if (strPos > strlen(str) - 1){
      strPos = 0;
      textCycleNow++;
      if (textCycleNow >= TEXT_CYCLE_COUNT) {
        textCycleNow = 0;
        textStage = false;
        wait = true;
      }
    }
    strLastCharIndex = 0;
    for (int i = 0 ; i < ALPHABET_COUNT; i++) {
      if ((char)pgm_read_byte(&str[strPos]) == (char)pgm_read_byte(&alphabet[i])) {
        strLastCharIndex = i;
      }
    }
    for (uint8_t i = 0; i < 7; i++) {
      textView[i][7] = (pgm_read_byte(&chars[strLastCharIndex * 7 + i]) >> strCharPos) & 1;
    }
  } else {
    for (uint8_t i = 0; i < STRING_WIDTH; i++) {
      textView[i][7] = 0;
    }
  }
}

void textToLeds() {
  for (uint8_t i = 0; i < 7; i++) {
    if (i % 2) {
      for (int j = 0; j < 8; j++)
        leds[(31 - i) * 8 + j] = (textView[i][j]) ? textColor : CRGB::Black;
    } else {
      for (int j = 0; j < 8; j++)
        leds[(31 - i) * 8 + j] = (textView[i][7 - j]) ? textColor : CRGB::Black;
    }
  }
}

void textMove2() {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8 - 1; j++) {
      textView2[i][j] = textView2[i][j + 1];
    }
  }
  
  strCharPos2++;
  if (strCharPos2 < pgm_read_byte(&alphabetLengths[strLastCharIndex2])) {
    for (uint8_t i = 0; i < 7; i++) {
      textView2[i][7] = (pgm_read_byte(&chars[strLastCharIndex2 * 7 + i]) >> strCharPos2) & 1;
    }
  } else if (strCharPos2 > pgm_read_byte(&alphabetLengths[strLastCharIndex2]) + CHAR_SPACE) {
    strCharPos2 = 0;
    strPos2++;
    if (strPos2 > strlen(str2) - 1){
      strPos2 = 0;
      if (textStage == false)
        wait2 = true;
    }
    strLastCharIndex2 = 0;
    for (int i = 0 ; i < ALPHABET_COUNT; i++) {
      if ((char)pgm_read_byte(&str2[strPos2]) == (char)pgm_read_byte(&alphabet[i])) {
        strLastCharIndex2 = i;
        
      }
    }
    for (uint8_t i = 0; i < 7; i++) {
      textView2[i][7] = (pgm_read_byte(&chars[strLastCharIndex2 * 7 + i]) >> strCharPos2) & 1;
    }
  } else {
    for (uint8_t i = 0; i < STRING_WIDTH; i++) {
      textView2[i][7] = 0;
    }
  }
}

void textToLeds2() {
  for (uint8_t i = 0; i < 7; i++) {
    if (i % 2) {
      for (int j = 0; j < 8; j++)
        leds[(19 - i) * 8 + j] = (textView2[i][j]) ? textColor2 : CRGB::Black;
    } else {
      for (int j = 0; j < 8; j++)
        leds[(19 - i) * 8 + j] = (textView2[i][7 - j]) ? textColor2 : CRGB::Black;
    }
  }
}

void textMove3() {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 8 - 1; j++) {
      textView3[i][j] = textView3[i][j + 1];
    }
  }
  
  strCharPos3++;
  if (strCharPos3 < pgm_read_byte(&alphabetLengths[strLastCharIndex3])) {
    for (uint8_t i = 0; i < 7; i++) {
      textView3[i][7] = (pgm_read_byte(&chars[strLastCharIndex3 * 7 + i]) >> strCharPos3) & 1;
    }
  } else if (strCharPos3 > pgm_read_byte(&alphabetLengths[strLastCharIndex3]) + CHAR_SPACE) {
    strCharPos3 = 0;
    strPos3++;
    if (strPos3 > strlen(str3) - 1){
      strPos3 = 0;
      if (textStage == false)
        wait3 = true;
    }
    strLastCharIndex3 = 0;
    for (int i = 0 ; i < ALPHABET_COUNT; i++) {
      if ((char)pgm_read_byte(&str3[strPos3]) == (char)pgm_read_byte(&alphabet[i])) {
        strLastCharIndex3 = i;
        
      }
    }
    for (uint8_t i = 0; i < 7; i++) {
      textView3[i][7] = (pgm_read_byte(&chars[strLastCharIndex3 * 7 + i]) >> strCharPos3) & 1;
    }
  } else {
    for (uint8_t i = 0; i < STRING_WIDTH; i++) {
      textView3[i][7] = 0;
    }
  }
}

void textToLeds3() {
  for (uint8_t i = 0; i < 7; i++) {
    if (i % 2) {
      for (int j = 0; j < 8; j++)
        leds[(7 - i) * 8 + j] = (textView3[i][j]) ? textColor3 : CRGB::Black;
    } else {
      for (int j = 0; j < 8; j++)
        leds[(7 - i) * 8 + j] = (textView3[i][7 - j]) ? textColor3 : CRGB::Black;
    }
  }
}

void lineToLeds() {
  for (int i = 0; i < 8; i++)
    leds[10 * 8 + i] = 0x00FFFF;
  for (int i = 0; i < 8; i++)
    leds[22 * 8 + i] = 0x00FFFF;
}

CRGB getColor(uint8_t b){
  if (b == 255)
    return 0xFFFFFF;
  if (b == 1)
    return 0x606060;
  if (b == 2)
    return 0x0080FF;
  if (b == 3)
    return 0x343434;
  if (b == 4)
    return 0xFFFF00;
  return 0;
}

void animToLeds() {
  if (animPos % 2) {
    for (int j = 0; j < 8; j++)
      leds[animPos * 8 + j] = getColor(pgm_read_byte(&prop[(animPos % 2) * 8 + j]));
  } else {   
    for (int j = 0; j < 8; j++)
      leds[animPos * 8 + j] = getColor(pgm_read_byte(&prop[(animPos % 2) * 8 + 7 - j]));
  }
  
  for (int i = 0; i < animPos - 15; i++) {
    if (i % 2) {
      for (int j = 0; j < 8; j++){
        leds[i * 8 + j] = getColor(pgm_read_byte(&smoke[j]));
        if ((animPos - i)*6 <= 255)
          leds[i * 8 + j].fadeToBlackBy((animPos - i)*6);
        else
          leds[i * 8 + j] = 0;
      }
    } else {
      for (int j = 0; j < 8; j++) {
        leds[i * 8 + j] = getColor(pgm_read_byte(&smoke[7 - j]));
        if ((animPos - i)*6 <= 255)
          leds[i * 8 + j].fadeToBlackBy((animPos - i)*6);
        else
          leds[i * 8 + j] = 0;
      }
    }
  }
  
  for (int i = 0; i < 16; i++) {
    if (i + animPos - 16 >= 0 && i + animPos - 16 <= 31){
      if (i % 2 && animPos % 2) {
        for (int j = 0; j < 8; j++)
          leds[(i - 16 + animPos) * 8 + j] = getColor(pgm_read_byte(&plane[(15 - i) * 8 + j]));
      } else if (!(i % 2) && animPos % 2) {
        for (int j = 0; j < 8; j++)
          leds[(i - 16 + animPos) * 8 + j] = getColor(pgm_read_byte(&plane[(15 - i) * 8 + 7 - j]));
      } else if ((i % 2) && !(animPos % 2)) {
        for (int j = 0; j < 8; j++)
          leds[(i - 16 + animPos) * 8 + j] = getColor(pgm_read_byte(&plane[(15 - i) * 8 + 7 - j]));
      } else {
        for (int j = 0; j < 8; j++)
          leds[(i - 16 + animPos) * 8 + j] = getColor(pgm_read_byte(&plane[(15 - i) * 8 + j]));
      }
    }
  }
}

void showAnim() {
  if (millis() > timeranim) {
    if (animPos == 0) {
      timeranim = millis() + TIMERANIM_MOVE;
    }
    timeranim += TIMERANIM_MOVE;
    animPos++;
    //Serial.println(animPos);
    //delay(1);
    if (animPos >= 71) {//32+16+16+16-1 = 71
      if (ledstrip == 1) {
        ledstrip = 0;
        digitalWrite(PIN_LEDSTRIP_L, 0);
        digitalWrite(PIN_LEDSTRIP_R, 1);
      } else {
        ledstrip = 1;
        digitalWrite(PIN_LEDSTRIP_R, 0);
        digitalWrite(PIN_LEDSTRIP_L, 1);
      }
      textStage = true;
      wait = false;
      timertext = millis() + TIMERTEXT_MOVE;
      wait2 = false;
      timertext2 = millis() + TIMERTEXT_MOVE2;
      wait3 = false;
      timertext3 = millis() + TIMERTEXT_MOVE3;
      textCycleNow = 0;
      animPos = 0;
    }
  }
  wipe();
  animToLeds();
}

void setup() {
  Serial.begin(57600);
  pinMode(PIN_LEDSTRIP, OUTPUT);
  pinMode(PIN_LEDSTRIP_L, OUTPUT);
  digitalWrite(PIN_LEDSTRIP_L, 1);
  pinMode(PIN_LEDSTRIP_R, OUTPUT);
  digitalWrite(PIN_LEDSTRIP_R, 0);
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(1000);
  digitalWrite(13, 0);
  FastLED.addLeds<WS2812, PIN_LEDSTRIP, GRB>(leds, LEDS_COUNT);
  timertext = millis() + TIMERTEXT_MOVE;

  strPos = 0;
  strCharPos = 0;
  strLastCharIndex = 0;
  strPos2 = 0;
  strCharPos2 = 0;
  strLastCharIndex2 = 0;
  strPos3 = 0;
  strCharPos3 = 0;
  strLastCharIndex3 = 0;
  for (int i = 0; i < ALPHABET_COUNT - 1; i++)
    if ((char)pgm_read_byte(&str[strPos]) == (char)pgm_read_byte(&alphabet[i]))
      strLastCharIndex = i;
  for (int i = 0; i < ALPHABET_COUNT - 1; i++)
    if ((char)pgm_read_byte(&str2[strPos2]) == (char)pgm_read_byte(&alphabet[i]))
      strLastCharIndex2 = i;
  for (int i = 0; i < ALPHABET_COUNT - 1; i++)
    if ((char)pgm_read_byte(&str3[strPos3]) == (char)pgm_read_byte(&alphabet[i]))
      strLastCharIndex3 = i;
}

void loop() {
  if (millis() > timertext){
    timertext = millis() + TIMERTEXT_MOVE;
    if (!wait)
      textMove();
  }
  if (millis() > timertext2){
    timertext2 = millis() + TIMERTEXT_MOVE2;
    if (!wait2)
      textMove2();
  }
  if (millis() > timertext3){
    timertext3 = millis() + TIMERTEXT_MOVE3;
    if (!wait3)
      textMove3();
  }
  //wipetext();
  if (!wait)
    textToLeds();
  if (!wait2)
    textToLeds2();
  if (!wait3)
    textToLeds3();
  if (!wait || !wait2 || !wait3)
    lineToLeds();
  if (wait && wait2 && wait3)
    showAnim();
  leds.fadeToBlackBy(255 - BRIGHTNESS);
  FastLED.show();
  delay(1);
}
