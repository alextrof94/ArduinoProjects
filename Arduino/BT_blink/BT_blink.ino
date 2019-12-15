/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

uint32_t timerOn, timerOff;
bool tOn, tOff;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  timerOn = micros() + 1000000;
  timerOff = timerOn + 5000;
}

// the loop function runs over and over again forever
void loop() {
  
  if (micros() > timerOn){
    timerOn += 1000000;
    tOn = 1;
  }
  
  if (micros() > timerOff){
    timerOff += 1000000;
    tOff = 1;
  }
  
  if (tOn){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  
  if (tOff){
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println();
    Serial.println("HELLO, I'M BT MEGA 2560 PRO");
    Serial.println(timerOn);
    Serial.println(timerOff);
  }
  
    tOn = 0;
    tOff = 0;
}
