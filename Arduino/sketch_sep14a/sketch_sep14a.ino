void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(11, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (!digitalRead(3))
      Serial.println(3);
    if (!digitalRead(4))
      Serial.println(4);
    if (digitalRead(11))
      Serial.println(11);
      Serial.println();
  delay(100);
}
