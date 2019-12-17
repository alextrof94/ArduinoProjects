
#include <Wire.h>
#include <TM1638.h>
#include <TM16XXFonts.h>
#include <larttm1638.h>
  
  void LART_TM1638 :: setDisplayDigit( byte dig , byte pos , boolean dot , const byte numberFont[]  )
  {
    if( pos > 7 ) pos = 7;
    byte bitPos = 1 << pos;
    byte bitScan = 1;
    byte Ch = numberFont[ dig ] | ( dot ? 1 << 7 : 0 );
    for( byte i = 0 ; i < 8 ; i++ )
    {
      buf[ i ] |= ( Ch & bitScan ) ? bitPos : 0;
      bitScan <<= 1;      
    }
  }
  
  void LART_TM1638 :: clearDisplayDigit( byte pos , boolean dot )
  {
    if( pos > 7 ) pos = 7;
    byte bitPos =~( 1 << pos );
    //if ( !dot ) bitPos &= 0x7F;
    for( byte i = 0 ; i < 8 ; i++ )
      buf[ i ] &= bitPos;
  }
  void LART_TM1638 :: setDisplayToHexNumber(unsigned long number, byte dots, boolean leadingZeros , const byte numberFont[] )
  {
    memset( buf , 0 , 8 );
    byte scanDot = 1;
    for (int i = 0; i < displays; i++) {
	if (!leadingZeros && number == 0) {
	} else {
		setDisplayDigit(number & 0xF, displays - i - 1 , dots & scanDot );
		number >>= 4;
    }
    scanDot <<= 1;
  }
  setDisplay( );
}
  
  //void LART_TM1638 :: setDisplay( )
  //{
  //  TM1638 :: setDisplay( buf );
 // }
  
  void LART_TM1638 :: setDisplayToDecNumberAt(unsigned long number, byte dots, byte startingPos , boolean leadingZeros ,
	const byte numberFont[] )
{
    byte scanDot = 1;  
    for (int i = 0; i < displays - startingPos; i++) 
    {
      clearDisplayDigit(displays - i - 1, (dots & (1 << i)) != 0);
      if ( ( number != 0 ) || leadingZeros ) 
      {
        setDisplayDigit(number % 10, displays - i - 1, ( dots & scanDot ) );
        scanDot <<= 1;
        number /= 10;
      } 
    }
  setDisplay( );
}
  void LART_TM1638 :: setDisplayToDecNumber(unsigned long number, byte dots, boolean leadingZeros,
	const byte numberFont[])
{
	setDisplayToDecNumberAt(number, dots, 0, leadingZeros, numberFont);
}

  void LART_TM1638 :: setDisplayString( char* str )
  {
    byte pos = 0;
    memset( buf , 0 , 8 );
    for( int i = 0 ; pos < displays && str[ i ] != 0 ; i++ , pos++ )
    {
      if ( str[ i+1 ] == '.' )
      {
        setDisplayDigit( str[ i ] - 32 , pos , true , FONT_DEFAULT );
        ++i;
      }
      else
      {
        setDisplayDigit( str[ i ] - 32, pos , false , FONT_DEFAULT );
      }
      
    }
    setDisplay( );
  }
unsigned long LART_TM1638::getKeys(void)
{
  unsigned long keys = 0;

  digitalWrite(strobePin, LOW);
  send(0x42);
  for (int i = 0; i < 4; i++) {
    keys *= 0x100;
	keys |= receive( );
  }
  digitalWrite(strobePin, HIGH);

  return keys;
}
