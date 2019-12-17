/*
 *
 */
#include <TM1638.h>
#include <larttm1638.h>

LART_TM1638 module(8, 9, 7);

void setup() 
{
} 
  
unsigned long counter = 0;

void loop() 
{
  char str[16];
  sprintf( str , "SEC= %4d" , counter );
   module.setDisplayString( str );
   delay( 1000 );
}

