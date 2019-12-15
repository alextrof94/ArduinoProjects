// ДЕФАЙНЫ или МАКРОСЫ начинаются со слова #define, затем идет название макроса и его определение, например:
#define PIN_LEDR 4
#define PIN_LEDY 5
#define PIN_LEDG 6
#define PIN_BTN 12

#define DELAY 100

// Переменная хранит в себе какое-то значение. Объявлять можно несколько переменных сразу, через запятую.
// Сначала идет тип переменной, затем название переменной, после может идти ее определение, например:
int a, b, c;
bool q = false;
bool btn, btnOld;

// Функции. Функции - это программы, которые можно вызвать использовав их имя.
// Самые простые функции, которые есть в любой программе - это setup и loop.
// У функции есть тип возвращаемого значения, имя, параметры и тело. Если тип возвращаемого значения не void - значит, в теле функции должна быть команда return.
// Рассмотрим пример:
// bool - тип возвращаемого значения
// getButton - имя функции
// В скобках "int pin" - параметр с типом, который требуется передать в функцию, в данном случае - это пин, который требуется передать в функцию, чтобы получить значение на нем.
// Внутри {} фигурных скобок - тело функции.
bool getButton(int pin) {
  bool value; // объявляем локальную переменную
  value = digitalRead(pin); // считываем значение с пина в переменную
  value = !value; // переворачиваем значение (было 1 - стало 0, или наоборот)
  return value; // возвращаем значение переменной вовне
}

void setup() {
  // Функция setup выполняется единожды, при старте микроконтроллера
  pinMode(PIN_LEDR, OUTPUT); // выставляем режим для пина светодиода, вывод
  pinMode(PIN_LEDY, OUTPUT);
  pinMode(PIN_LEDG, OUTPUT);
  
  pinMode(PIN_BTN, INPUT_PULLUP); // Выставляем режим для пина кнопки,  ввод подтянутый к плюсу

  Serial.begin(57600);
}

// Сравнение <===
// == - равно ли
// != - не равно ли
// <  - меньше ли
// <= - меньше или равно ли
// >  - больше ли
// >= - больше или равно ли

bool grlndOn = false;

void loop() {
  Serial.print("LOOP");
  Serial.println(random(0,9999));
  // Функция loop повторяется бесконечно после setup
  btn = getButton(PIN_BTN); // с помощью нашей функции получаем значение кнопки и записываем в переменную btn, которую объявили в самом начале
  if (btn == true){
    if (btnOld == false) {
      grlndOn = !grlndOn;
      btnOld = true;
    }
  } else {
    if (btnOld == true)
      btnOld = false;
  }
  
  if (grlndOn == true) {
      digitalWrite(PIN_LEDR, 1);
      delay(DELAY);
      digitalWrite(PIN_LEDR, 0);
      
      digitalWrite(PIN_LEDY, 1);
      delay(DELAY);
      digitalWrite(PIN_LEDY, 0);
      
      digitalWrite(PIN_LEDG, 1);
      delay(DELAY);
      digitalWrite(PIN_LEDG, 0);    
  }
}
