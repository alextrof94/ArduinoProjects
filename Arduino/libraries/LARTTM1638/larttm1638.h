/**
 * ���������� ��� ����������� �� ���� TM1638 
 * � �������� ���������� ����� COL � ROW 
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
   * �����������
   */
  LART_TM1638( int pinData , int pinClk , int pinStrob ) : TM1638( pinData , pinClk , pinStrob ){ }
  /**
   * ����� �����
   * @param dig - �����
   * @param pos - ��������� ������� �� ������
   * @param dot - ������� ����� �����
   * @param numberFont - ����� ��� ����������
   */
   void setDisplayDigit( byte dig , byte pos , boolean dot = false , const byte numberFont[] = NUMBER_FONT );  
  /**
   * ������� ��������� 
   * @param pos - ����� ����������
   * @param dot - ������� ����� �����
   */
  void clearDisplayDigit( byte pos , boolean dot );
  /**
   * ����� ����� � HEX
   * @param number - �����
   * @param dots - ������� ����� �����
   * @param leadingZeros - ������� ����
   * @param numberFont - ����� ��� ����������
   */  
  void setDisplayToHexNumber(unsigned long number, byte dots, boolean leadingZeros = true, const byte numberFont[] = NUMBER_FONT);  
   /**
	* ���������� ������
    */  
  void setDisplay( ) {  TM1638 :: setDisplay( buf );  }
  /**
   * ����� ����� � DEC
   * @param number - �����
   * @param startingPos - ��������� ������� �� ������
   * @param dots - ������� ����� �����
   * @param leadingZeros - ������� ����
   * @param numberFont - ����� ��� ����������
   */    
  void setDisplayToDecNumberAt(unsigned long number, byte dots, byte startingPos = 0, boolean leadingZeros = true,
	const byte numberFont[] = NUMBER_FONT);
  /**
   * ����� ����� � DEC
   * @param number - �����
   * @param dots - ������� ����� �����
   * @param leadingZeros - ������� ����
   * @param numberFont - ����� ��� ����������
   */
  void setDisplayToDecNumber(unsigned long number, byte dots, boolean leadingZeros,
	const byte numberFont[]);
  /**
   * ����� ����� � ������
   * @param str - ������
   */
  void setDisplayString( char* str );
  
  unsigned long getKeys(void);
  
};
#endif