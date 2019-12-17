/**
 * БИБЛИОТЕКА ДЛЯ ИНДИКАТОРОВ НА ЧИПЕ TM1638 
 * С ОБРАТНЫМ ВКЛЮЧЕНИЕМ ЛИНИЙ COL И ROW 
 */
#ifndef LART_TM1638_H
#define LART_TM1638_H

#include <Arduino.h>
#include <TM1638.h>


class LART_TM1638 : public TM1638
{
  private:
  byte buf[ 8 ];
  public:
  /**
   * Конструктор
   */
  LART_TM1638( int pinData , int pinClk , int pinStrob ) : TM1638( pinData , pinClk , pinStrob ){ }
  /**
   * Вывод числа
   * @param dig - число
   * @param pos - начальная позиция на экране
   * @param dot - битовая маска точек
   * @param numberFont - шрифт для распечатки
   */
   void setDisplayDigit( byte dig , byte pos , boolean dot = false , const byte numberFont[] = NUMBER_FONT );  
  /**
   * Очистка знакомета 
   * @param pos - номер занкоместа
   * @param dot - битовая маска точек
   */
  void clearDisplayDigit( byte pos , boolean dot );
  /**
   * Вывод числа в HEX
   * @param number - число
   * @param dots - битовая маска точек
   * @param leadingZeros - ведущие нули
   * @param numberFont - шрифт для распечатки
   */  
  void setDisplayToHexNumber(unsigned long number, byte dots, boolean leadingZeros = true, const byte numberFont[] = NUMBER_FONT);  
   /**
	* Обновление экрана
    */  
  void setDisplay( ) {  TM1638 :: setDisplay( buf );  }
  /**
   * Вывод числа в DEC
   * @param number - число
   * @param startingPos - начальная позиция на экране
   * @param dots - битовая маска точек
   * @param leadingZeros - ведущие нули
   * @param numberFont - шрифт для распечатки
   */    
  void setDisplayToDecNumberAt(unsigned long number, byte dots, byte startingPos = 0, boolean leadingZeros = true,
	const byte numberFont[] = NUMBER_FONT);
  /**
   * Вывод числа в DEC
   * @param number - число
   * @param dots - битовая маска точек
   * @param leadingZeros - ведущие нули
   * @param numberFont - шрифт для распечатки
   */
  void setDisplayToDecNumber(unsigned long number, byte dots, boolean leadingZeros,
	const byte numberFont[]);
  /**
   * Вывод числа в строки
   * @param str - строка
   */
  void setDisplayString( char* str );
  
  unsigned long getKeys(void);
  
};
#endif