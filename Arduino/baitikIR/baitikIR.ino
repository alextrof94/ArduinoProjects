// библиотека для приемника
#include <IRremote.h>

// команды принятые с пульта управления при помощи монитора порта
#define IRCMD_F 0xFD8877 // forward
#define IRCMD_L 0xFD28D7 // left
#define IRCMD_R 0xFD6897 // right
#define IRCMD_B 0xFD9867 // back
#define IRCMD_S 0xFDA857 // stop

// пины для управления моторами
const int in1 = 4; // IN4 pin 2
const int in2 = 3; // IN3 pin 4
const int in3 = 2; // IN2 pin 5
const int in4 = 5; // IN1 pin 7

// настройка приемника
int RECV_PIN = 11; // пин приемника
int IR5V_PIN = 12; // пин для питания приемника
IRrecv irrecv(RECV_PIN); // классы для работы приемника из библиотеки
decode_results results; 

void setup() {
  pinMode(in1, OUTPUT); // выход на L298n
  pinMode(in2, OUTPUT); // выход на L298n
  pinMode(in3, OUTPUT); // выход на L298n
  pinMode(in4, OUTPUT); // выход на L298n
  pinMode(13, OUTPUT); // светодиод

  // Хак, если надо запитать какое-то малопотребляющее устройство - втыкаем его в цифровой пин и выдаем на пин высокий уровень
  // Не вздумайте таким образом питать двигатели или даже несколько светодиодов одновременно - порт (8 пинов) или даже весь микроконтроллер могут сгореть
  pinMode(IR5V_PIN, OUTPUT); 
  digitalWrite(IR5V_PIN, HIGH);
  
  irrecv.enableIRIn(); // Включаем прием сигналов с пульта
  Serial.begin(57600); // Это для вывода команд в монитор порта
}

void loop() {
  // получаем команду с пульта 
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); // выводим полученную команду в монитор порта
    irrecv.resume(); // Можем принимать следующую команду
  }
  // если команда равна той, что мы записали заранее - делаем вещи
  if (results.value == IRCMD_F) 
  {
    // едем вперед
    digitalWrite (in1, LOW);
    digitalWrite (in2, HIGH); 
    digitalWrite (in3, HIGH); 
    digitalWrite (in4, LOW);
    // Мигнем светодиодом о том, что команда принята
    digitalWrite(13, HIGH); 
    delay(100);
    digitalWrite(13, LOW);
  }
  if (results.value == IRCMD_B) 
  {
    // едем назад
    digitalWrite (in1, HIGH);
    digitalWrite (in2, LOW); 
    digitalWrite (in3, LOW); 
    digitalWrite (in4, HIGH);
    // Мигнем светодиодом о том, что команда принята
    digitalWrite(13, HIGH); 
    delay(100);
    digitalWrite(13, LOW);
  }
  if (results.value == IRCMD_R) 
  {
    // поворачиваем направо
    digitalWrite (in1, LOW);
    digitalWrite (in2, HIGH); 
    digitalWrite (in3, LOW); 
    digitalWrite (in4, HIGH);
    // Мигнем светодиодом о том, что команда принята
    digitalWrite(13, HIGH); 
    delay(100);
    digitalWrite(13, LOW);
  }
  if (results.value == IRCMD_L) 
  {
    // поворачиваем налево
    digitalWrite (in1, HIGH);
    digitalWrite (in2, LOW); 
    digitalWrite (in3, HIGH); 
    digitalWrite (in4, LOW);
    // Мигнем светодиодом о том, что команда принята
    digitalWrite(13, HIGH); 
    delay(100);
    digitalWrite(13, LOW);
  }
  if (results.value == IRCMD_S) 
  {
    // стоп
    digitalWrite (in1, LOW);
    digitalWrite (in2, LOW); 
    digitalWrite (in3, LOW); 
    digitalWrite (in4, LOW);
    // Мигнем светодиодом о том, что команда принята
    digitalWrite(13, HIGH); 
    delay(100);
    digitalWrite(13, LOW);
  }

}
