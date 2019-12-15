#define Trig 8
#define Echo 9
const int in1 = 3; // IN4 pin 2
const int in2 = 7; // IN3 pin 4
const int in3 = 5; // IN2 pin 5
const int in4 = 4; // IN1 pin 7

void setup()
{
  pinMode(Trig, OUTPUT); // выход
  pinMode(Echo, INPUT); // вход
  pinMode(in1, OUTPUT); // выход на L298n
  pinMode(in2, OUTPUT); // выход на L298n
  pinMode(in3, OUTPUT); // выход на L298n
  pinMode(in4, OUTPUT); // выход на L298n
}

float getCM(){
  float duration, distance; // Объявляем переменные
  digitalWrite(Trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(Trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(Trig, LOW);
  duration = pulseIn(Echo, HIGH); // Считываем длительность
  distance = duration / 58; // Переводим в сантиметры
  return distance;
}

unsigned int impulseTime=0;
unsigned int distance_sm=0;

void loop()
{
  distance_sm = getCM();
  if (distance_sm > 20) // если расстояние более 20 сантиметров
  {
    digitalWrite (in1, HIGH);
    digitalWrite (in2, LOW); 
    digitalWrite (in3, LOW); 
    digitalWrite (in4, HIGH);
    delay(200);
  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2000);
  
  }
}
