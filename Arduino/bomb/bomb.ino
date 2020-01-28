
void setup() {
  for (int i = 2; i < 7; i++)
    pinMode(i, OUTPUT);

  for (int i = 2; i < 7; i++){
    digitalWrite(i, 1);
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
