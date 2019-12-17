#define PIN_MOTOR_A_IN1 2
#define PIN_MOTOR_A_IN2 4
#define PIN_MOTOR_B_IN1 7
#define PIN_MOTOR_B_IN2 8
#define PIN_MOTOR_A_EN 5
#define PIN_MOTOR_B_EN 6

#define PIN_US_TRIG 13
#define PIN_US_ECHO 12

#define PIN_SERVO 3

#define PIN_LINE_D_L 9
#define PIN_LINE_D_C 10
#define PIN_LINE_D_R 11

#define PIN_LINE_A_L A0
#define PIN_LINE_A_C A1
#define PIN_LINE_A_R A2

#define PIN_LEDS A3

#include <Servo.h>
Servo myservo; 

#include "FastLED.h"
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];

int getCm() {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);
  return (pulseIn(PIN_US_ECHO, HIGH) / 2) / 29.1;
}

void setup() {
  Serial.begin(57600);
  pinMode(PIN_MOTOR_A_IN1, OUTPUT);
  pinMode(PIN_MOTOR_A_IN2, OUTPUT);
  pinMode(PIN_MOTOR_B_IN1, OUTPUT);
  pinMode(PIN_MOTOR_B_IN2, OUTPUT);
  pinMode(PIN_MOTOR_A_EN, OUTPUT);
  pinMode(PIN_MOTOR_B_EN, OUTPUT);
  digitalWrite(PIN_MOTOR_A_IN1, 1);
  digitalWrite(PIN_MOTOR_A_IN2, 0);
  digitalWrite(PIN_MOTOR_B_IN1, 1);
  digitalWrite(PIN_MOTOR_B_IN2, 0);

  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  
  pinMode(PIN_SERVO, OUTPUT);

  pinMode(PIN_LINE_D_L, INPUT);
  pinMode(PIN_LINE_D_C, INPUT);
  pinMode(PIN_LINE_D_R, INPUT);
  pinMode(PIN_LINE_A_L, INPUT);
  pinMode(PIN_LINE_A_C, INPUT);
  pinMode(PIN_LINE_A_R, INPUT);
  
  myservo.attach(PIN_SERVO);
  
  FastLED.addLeds<WS2812, PIN_LEDS, GRB>(leds, NUM_LEDS); 
}

void loop() {
  if (analogRead(PIN_LINE_A_L) > 512){
    leds[11] = CRGB(255,0,0);
  } else
    leds[11] = 0;
  if (analogRead(PIN_LINE_A_C) > 512){
    leds[1] = CRGB(255,0,0);
  } else
    leds[1] = 0;
  if (analogRead(PIN_LINE_A_R) > 512){
    leds[3] = CRGB(255,0,0);
  } else
    leds[3] = 0;
  FastLED.show();
  /*
  for (int i = 0; i < 180; i++){
    myservo.write(i);
    delay(10);
    Serial.println(getCm());
  }
  for (int i = 0; i < 180; i++){
    myservo.write(180-i);
    delay(10);
    Serial.println(getCm());
  }
  */
  /*
  for (uint8_t i = 0; i < 255; i++){
    analogWrite(PIN_MOTOR_A_EN, i);
    analogWrite(PIN_MOTOR_B_EN, i);
    delay(10);
  }
  digitalWrite(PIN_MOTOR_A_EN, 1);
  digitalWrite(PIN_MOTOR_B_EN, 1);
  delay(1000);
  for (uint8_t i = 255; i > 0; i--){
    analogWrite(PIN_MOTOR_A_EN, i);
    analogWrite(PIN_MOTOR_B_EN, i);
    delay(10);
  }
  digitalWrite(PIN_MOTOR_A_EN, 0);
  digitalWrite(PIN_MOTOR_B_EN, 0);
  delay(1000);
  */
}
