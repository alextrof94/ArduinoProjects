void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3, 1);
  delay(100);
  digitalWrite(3, 0);
  delay(900);

}
