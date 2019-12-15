#include <Servo.h>
#include <SoftwareSerial.h>

#define PIN_MOTOR_FRONT 5
#define PIN_MOTOR_BACK 4
#define PIN_MOTOR_ENABLE 3
#define PIN_SERVO 6
#define PIN_BT_RX A0
#define PIN_BT_TX A1
#define PIN_LEDL 12
#define PIN_LEDR 11
#define PIN_BUZZER 10
#define PIN_POWERINDICATOR 9
#define PIN_LED 13
#define PIN_POWERCHECK A4

#define SETTINGS_MAX_SPEED 255
#define SETTINGS_BUZZER_DELAY 500
#define SETTINGS_BLINK_DELAY 500
#define SETTINGS_IDLEBLINK_DELAY 100

#define SETTINGS_START_POWERINDICATOR_DELAY 1000
#define SETTINGS_POWER_LOW_CUTOFF 9.0f

Servo myservo;
SoftwareSerial BT(PIN_BT_RX, PIN_BT_TX); // RX, TX

bool buzzerEnabled = false;
uint32_t buzzerTimer = 0;
bool buzzerState = false;
bool blinkEnabled = false;
uint32_t blinkTimer = 0;
bool blinkState = false;
uint32_t idleTimer = 0;
bool idleState = false;
bool powerIndicatorEnabled = true;
bool powerIndicatorStartState = true;

char BTData[20];
uint8_t BTDataIndex = 0;

void setDir(bool f){
  if (f) {
    digitalWrite(PIN_MOTOR_FRONT, 1);
    digitalWrite(PIN_MOTOR_BACK, 0);
  } else {
    digitalWrite(PIN_MOTOR_FRONT, 0);
    digitalWrite(PIN_MOTOR_BACK, 1);
  }
}


void setSpd(int s){
  if (s >= 0) {
    setDir(1);
  } else {
    setDir(0);
    s = -s;
  }
  analogWrite(PIN_MOTOR_ENABLE, (s / 255.0) * SETTINGS_MAX_SPEED);
}

void setRot(float r){
  myservo.write((r * 120.0));
}


void BTDataWork() {
  if (BTData[0] == 'r'){ // rotate
    if (BTData[1] == '0')
      setRot(0);
    if (BTData[1] == '1')
      setRot(0.5f);
    if (BTData[1] == '2')
      setRot(1);
      /*
    char buffer[4];
    buffer[0] = BTData[1];
    buffer[1] = BTData[2];
    buffer[2] = BTData[3];
    buffer[3] = '\0';
    setRot(atoi(buffer));*/
  }
  if (BTData[0] == 's'){ // speed
    char buffer[4];
    buffer[0] = BTData[1];
    buffer[1] = BTData[2];
    buffer[2] = BTData[3];
    buffer[3] = '\0';
    setSpd(atoi(buffer));
  }
  if (BTData[0] == 'b'){ // speed
    char buffer[4];
    buffer[0] = BTData[1];
    buffer[1] = BTData[2];
    buffer[2] = BTData[3];
    buffer[3] = '\0';
    setSpd(-atoi(buffer));
  }
  if (BTData[0] == 'p'){ // police
    buzzerEnabled = (BTData[1] == '1');
    blinkEnabled = (BTData[1] == '1');
  }
  if (BTData[0] == 'i'){ // powerIndicator
    powerIndicatorEnabled = (BTData[1] == '1');
    BT.println();
    BT.print("POWER IS ");
    BT.println(powerGet());
    BT.print("POWER CUTOFF IS ");
    BT.println(powerCheckCutoff());
    BT.println();
  }
}

float powerGet() {
  return (analogRead(PIN_POWERCHECK) / 1024.0 * 50);
}

bool powerCheckCutoff(){
  return (powerGet() <= SETTINGS_POWER_LOW_CUTOFF);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTOR_FRONT, OUTPUT);
  pinMode(PIN_MOTOR_BACK, OUTPUT);
  pinMode(PIN_MOTOR_ENABLE, OUTPUT);
  pinMode(PIN_LEDR, OUTPUT);
  pinMode(PIN_LEDL, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_POWERINDICATOR, OUTPUT);
  myservo.attach(PIN_SERVO);
  BT.begin(38400);
  Serial.begin(57600);
  setRot(0.5f);
}

void loop() {
  if (BT.available()){
    char c = BT.read();
    if (c != '\r' && c != '\n'){
      BTData[BTDataIndex] = c;
      BTDataIndex++;
    } else {
      Serial.println(BTData);
      if (BTDataIndex > 0) 
        BTDataWork();
      BTDataIndex = 0;
    }
  }
  
  if (powerIndicatorStartState == true){
    if (millis() > SETTINGS_START_POWERINDICATOR_DELAY){
      powerIndicatorEnabled = false;
      powerIndicatorStartState = false;
    }
  }
  digitalWrite(PIN_POWERINDICATOR, powerIndicatorEnabled);
  digitalWrite(PIN_LED, powerIndicatorEnabled);

  
  if (powerCheckCutoff()) {
    digitalWrite(PIN_POWERINDICATOR, 1);
    if (buzzerTimer < millis()){
      buzzerTimer = millis() + SETTINGS_BUZZER_DELAY;
      buzzerState = !buzzerState;
      if (buzzerState){
        tone(PIN_BUZZER, 200);
      } else {
        tone(PIN_BUZZER, 300);
      }
    }
    if (blinkTimer < millis()){
      blinkTimer = millis() + SETTINGS_BLINK_DELAY;
      blinkState = !blinkState;
      digitalWrite(PIN_LEDR, blinkState);
      digitalWrite(PIN_LEDL, blinkState);
    }    
  } else {
    if (buzzerEnabled){
      if (buzzerTimer < millis()){
        buzzerTimer = millis() + SETTINGS_BUZZER_DELAY;
        buzzerState = !buzzerState;
        if (buzzerState){
          tone(PIN_BUZZER, 650);
        } else {
          tone(PIN_BUZZER, 1350);
        }
      }
    } else
      noTone(PIN_BUZZER);
          
    if (blinkEnabled){
      if (blinkTimer < millis()){
        blinkTimer = millis() + SETTINGS_BLINK_DELAY;
        blinkState = !blinkState;
        digitalWrite(PIN_LEDR, blinkState);
        digitalWrite(PIN_LEDL, !blinkState);
      }
    } else {
      if (idleTimer < millis()){
        idleTimer = millis() + SETTINGS_IDLEBLINK_DELAY;
        idleState = !idleState;
        digitalWrite(PIN_LEDR, idleState);
        digitalWrite(PIN_LEDL, 0);
      }
    }
  }
  
}
