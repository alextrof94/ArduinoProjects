//CustServo.h
#ifndef CustServo_h
#define CustServo_h

#include <Arduino.h>

class CustServo
{
  Stream & port_; // http://forum.arduino.cc/index.php?topic=190210.0#lastPost  Reply #14
  
  public:
  // constructor
    CustServo (Stream & port) : port_ (port) { }
  
  // methods
    void Number_of_Channels(uint8_t channel = 4);
    void Move(uint8_t channel, uint8_t position);
	void Calibrate();

  private:
    int left, right;
    unsigned int ServoNum[35];
    unsigned long time;
};
#endif
