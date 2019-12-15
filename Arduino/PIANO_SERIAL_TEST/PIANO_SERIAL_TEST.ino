void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial1.begin(31250);
  pinMode(13, OUTPUT);
  
    Serial.println("HELLO");
}
int opened = 0;
uint32_t t = 1000;
void loop() {
  // put your main code here, to run repeatedly:
  if (millis() > t){
    t += 1000;
    Serial.print("OPENED: ");
    Serial.println(opened);
  }
  if (Serial1.available() > 2) {
    digitalWrite(13, 1);
    byte cmd = Serial1.read();
    if (cmd == 159)
      opened++;
    if (cmd == 143)
      opened--;
    Serial.print(cmd, DEC);
    Serial.print(" ");
    Serial.print(Serial1.read(), DEC);
    Serial.print(" ");
    Serial.print(Serial1.read(), DEC);
    Serial.println();
    digitalWrite(13, 0);
  }
}
