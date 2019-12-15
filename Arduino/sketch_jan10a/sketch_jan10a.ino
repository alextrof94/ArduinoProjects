void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial2.begin(31250);
  Serial.begin(57600);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
}

uint32_t t = 0;
bool last = 0;
void loop() {
  if (Serial2.available()){
    uint8_t a = Serial2.read();
    Serial.println(a, DEC);
  }

}
