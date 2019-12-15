void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(300);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
  
}

void loop() {
  // put your main code here, to run repeatedly
  if (Serial.available()){
    if (Serial.read() == 'e') {
  digitalWrite(13, 1);
      
    }else {
  digitalWrite(13, 0);
      
    }
    
  }

}
