
#define PIN_US_TRIG 10
#define PIN_US_ECHO 11

#define PIN_MTR_IN1 2 // l
#define PIN_MTR_IN2 3 // l
#define PIN_MTR_IN3 4 // r
#define PIN_MTR_IN4 5 // r
#define PIN_MTR_SPEED1 6 // l
#define PIN_MTR_SPEED2 9 // r

#define SETTINGS_MAXSPEED 255 // программное ограничение скорости

void setDirection (boolean dir = 1, boolean mtr = 0) { 
  // Устанавливает направление движения мотора. 
  // Если (dir == 1) - мотор движется вперед, если 0 - назад. 
  // Если функция вызвана без параметра - мотор будет двигаться вперед.
  // mtr - выбирает какому из моторов надо установить направление, 0 - левый, 1 - правый
  if (mtr == 0) {
    if (dir == 1) { // Проверка dir на соответствие условию
      // Условие пройдено - делаем так, чтобы ток в микросхеме шел в одну сторону, для этого на INPUT1 выставляем 1, а на INPUT2 - 0.
      digitalWrite(PIN_MTR_IN1, 1);
      digitalWrite(PIN_MTR_IN2, 0);
    } else {
      // Условие НЕ пройдено - делаем так, чтобы ток в микросхеме шел в обратную сторону, для этого на INPUT1 выставляем 0, а на INPUT2 - 1.
      digitalWrite(PIN_MTR_IN1, 0);
      digitalWrite(PIN_MTR_IN2, 1);
    }
  } else {
    if (dir == 1) { // Проверка dir на соответствие условию
      // Условие пройдено - делаем так, чтобы ток в микросхеме шел в одну сторону, для этого на INPUT1 выставляем 1, а на INPUT2 - 0.
      digitalWrite(PIN_MTR_IN3, 1);
      digitalWrite(PIN_MTR_IN4, 0);
    } else {
      // Условие НЕ пройдено - делаем так, чтобы ток в микросхеме шел в обратную сторону, для этого на INPUT1 выставляем 0, а на INPUT2 - 1.
      digitalWrite(PIN_MTR_IN3, 0);
      digitalWrite(PIN_MTR_IN4, 1);
    }
  }
  // Если у микросхемы выставить на INPUT1 и INPUT2 одновременно нули или единицы - ток в микросхеме не будет течь ни в одну сторону -> мотор не будет вращаться.
}

void setSpeed(int spd = 0, bool mtr = 0) { 
  // Устанавливает скорость вращения мотора в процентах. Если не задан параметр spd - остановить мотор.
  // Реальная скорость при использовании ШИМ имеет квадратичную функцию. 
  // Я вывел такую функцию:
  // real speed = 255 - (0.025 * (100 - percent_speed)^2)
  // Подробнее на http://chkalovc.ru/wiki/index.php/Ардуино._Рассчет_ШИМ_для_скорости_в_процентах.
  // Масштабируем скорость:
  float pwm = 255 - ((100 - spd) * (100 - spd) * 0.025f); // Масштабируем указанную скорость в процентах в реальное значение ШИМ
  if (mtr == 0)
    analogWrite(PIN_MTR_SPEED1, pwm);
  else
    analogWrite(PIN_MTR_SPEED2, pwm);
}

void setMotor(int spd, bool mtr = 0) { 
  // Дополнительная функция, которая будет использовать две другие функции для более простого управления мотором. Если скорость положительна - вращаем мотор вперед с этой скоростью, если скорость отрицательна - вращаем мотор в обратную сторону.
  if (spd >= 0) { // Если скорость положительна
    setDirection(1, mtr); // Едь вперед
    setSpeed(spd, mtr); // С такой-то скоростью
  } else {
    spd = -spd; // переворачиваем скорость относительно 0, чтобы функция setSpeed на проверке скорости ниже 0 не обнулила нам скорость.
    setDirection(0, mtr); // Едь назад
    setSpeed(spd, mtr); // С такеой-то скоростью
  }
}

void wheelStartEV3(float rot, float spd){
  // -100, 100 = setMotor(-255,0) setMotor(255,1)
  // -50, 100 =  setMotor(0,0) setMotor(255,1)
  // 0, 100 =    setMotor(255,0) setMotor(255,1)
  // 10, 100 =   setMotor(255,0) setMotor(204,1)
  // 25, 100 =   setMotor(255,0) setMotor(127,1)
  // 50, 100 =   setMotor(255,0) setMotor(0,1)
  // 75, 100 =   setMotor(255,0) setMotor(-127,1)
  // 100, 100 =  setMotor(255,0) setMotor(-255,1)

  if (rot >= 0) {
    setMotor(spd, 0);
    if (rot <= 50)
      setMotor(((50-rot)/ 50.0) * spd, 1);
    else
      setMotor(((rot - 50)/ 50.0) * spd * -1, 1);
  } else {
    setMotor(spd, 1);
    if (rot <= 50)
      setMotor(((50-rot)/ 50.0) * spd, 0);
    else
      setMotor(((rot - 50)/ 50.0) * spd * -1, 0);    
  }
}

void wheelEV3(float rot, float spd, float seconds){
  wheelStartEV3(rot, spd);
  delay(seconds * 1000);
  wheelStartEV3(0, 0);
}

float getCM(){
  int duration;
  float distance; // Объявляем переменные для длительности движения импульсов в воздухе и, непосредственно расстояния
  // Далее идет некоторая магия, которая полностью описана в даташите датчика расстояния HC-SR04
  digitalWrite(PIN_US_TRIG, LOW); // Низкий уровень на Триггер
  delayMicroseconds(2); // Ждем 2 микросекунды
  digitalWrite(PIN_US_TRIG, HIGH); // Высокий уровень на Триггер
  delayMicroseconds(10); // Ждем 10 микросекунд
  digitalWrite(PIN_US_TRIG, LOW); // Низкий уровень на Триггер
  duration = pulseIn(PIN_US_ECHO, HIGH); // Считываем длительность, которая придет в виде байта
  distance = duration / 58; // Переводим в сантиметры
  return distance;
}

void setup() {
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  pinMode(PIN_MTR_IN1, OUTPUT);
  pinMode(PIN_MTR_IN2, OUTPUT);
  pinMode(PIN_MTR_IN3, OUTPUT);
  pinMode(PIN_MTR_IN4, OUTPUT);
  pinMode(PIN_MTR_SPEED1, OUTPUT);
  pinMode(PIN_MTR_SPEED2, OUTPUT);
}

float distance_cm;
void loop() {
  
  distance_cm = getCM();
  if (distance_cm > 20) { // если расстояние более 20 сантиметров
    setMotor(100,0);
    setMotor(100,1);
  } else {
    setMotor(0,0);
    setMotor(100,1);
    // wheelEV3(100, 30, 1000);
  }

}
