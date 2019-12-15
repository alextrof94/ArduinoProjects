void setup() {
  // put your setup code here, to run once:

  Serial.begin(31250);
}

uint8_t i = 0; 
void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(i);
  delay(100);
  i++;
}
