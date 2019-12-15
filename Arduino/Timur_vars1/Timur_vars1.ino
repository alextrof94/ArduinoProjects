

void blink() {
  
}

void setup() {
  // put your setup code here, to run once:

int a = -132;
 a = 2 + a;
  
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = digitalRead(2);
  digitalWrite(13, a);
}
