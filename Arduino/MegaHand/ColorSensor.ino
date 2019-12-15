#define PIN_CS_LED 2
#define PIN_CS_S0  53
#define PIN_CS_S1  52
#define PIN_CS_S2  50
#define PIN_CS_S3  51
#define PIN_CS_OUT 49

#define SETTINGS_LED_PWM 255

void ColorSensorLight(byte a) {
  analogWrite(PIN_CS_LED, a);  
}

Color ColorSensorReadColor() {
  ColorSensorLight(SETTINGS_LED_PWM);
  Color res;
  digitalWrite(PIN_CS_S0, 1);
  digitalWrite(PIN_CS_S1, 1);
  delay(1000);
  digitalWrite(PIN_CS_S2, 0);
  digitalWrite(PIN_CS_S3, 0);
  res.r = pulseIn(PIN_CS_OUT, digitalRead(PIN_CS_OUT) == HIGH ? LOW : HIGH);
  
  digitalWrite(PIN_CS_S2, 0);
  digitalWrite(PIN_CS_S3, 1);
  res.b = pulseIn(PIN_CS_OUT, digitalRead(PIN_CS_OUT) == HIGH ? LOW : HIGH);
  
  digitalWrite(PIN_CS_S2, 1);
  digitalWrite(PIN_CS_S3, 1);
  res.g = pulseIn(PIN_CS_OUT, digitalRead(PIN_CS_OUT) == HIGH ? LOW : HIGH);
  
  digitalWrite(PIN_CS_S0, 0);
  digitalWrite(PIN_CS_S1, 0);
  ColorSensorLight(0);
  return res;
}

SimpleColors getSimpleColor(Color c) {
  if (c.r < 10 && c.g < 10 && c.b < 11) // 5 6 8 // 6 8 10
    return SCYELLOW;
  if (c.r < 10 && c.r < c.g && c.r < c.b) // 8 14 11 // 9 8 11 // 9 16 12
    return SCRED;
  if (c.g < 15 && c.g < c.r && c.b >= 10) // 17 12 11
    return SCGREEN;
  if (c.b < 10 && c.b < c.r && c.b < c.g) // 15 11 6
    return SCBLUE;
  return SCNONE; // 15 16 17
}

void ColorSensorPrintColor(Color c) {
  Serial.print(c.r);
  Serial.print(" ");
  Serial.print(c.g);
  Serial.print(" ");
  Serial.print(c.b);
  Serial.print(" : ");
  SimpleColors sc = getSimpleColor(c);
  if (sc == SCRED)
    Serial.println("RED");
  if (sc == SCGREEN)
    Serial.println("GREEN");
  if (sc == SCBLUE)
    Serial.println("BLUE");
  if (sc == SCYELLOW)
    Serial.println("YELLOW");
  if (sc == SCPURPLE)
    Serial.println("PURPLE");
  if (sc == SCCYAN)
    Serial.println("CYAN");
  if (sc == SCWHITE)
    Serial.println("WHITE");
  if (sc == SCBLACK)
    Serial.println("BLACK");
  Serial.println();
}


void ColorSensorInit() {
  pinMode(PIN_CS_LED, OUTPUT);
  pinMode(PIN_CS_S0,  OUTPUT);
  pinMode(PIN_CS_S1,  OUTPUT);
  pinMode(PIN_CS_S2,  OUTPUT);
  pinMode(PIN_CS_S3,  OUTPUT);
  pinMode(PIN_CS_OUT, INPUT);
  ColorSensorLight(0);
}
