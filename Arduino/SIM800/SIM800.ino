#include <SoftwareSerial.h>
#include <TinyGPS++.h>
//SoftwareSerial SIM800(3,2);

SoftwareSerial GPS(5,4);
TinyGPSPlus Parser;

void setup() {
  Serial.begin(9600);
  GPS.begin(9600);
  Serial.println("STARTED!");
  //SIM800.begin(9600);
}


String lat  = "Unknown         ";
String lng  = "location        ";
String date = "Unknown date    ";
String time = "Unknown time    ";

String twoDigits(int x) {
  if(x < 10) return "0" + String(x);
  else return String(x);
}

void loop() {
  //if (SIM800.available())
    //Serial.write(SIM800.read());
  while(!GPS.available());
  while (GPS.available()) {
    char temp = GPS.read();
    Parser.encode(temp);
  }
  if(Parser.location.isValid()) {
      lat = "Lat: " + String(Parser.location.lat(), 6);
      lng = "Lng: " + String(Parser.location.lng(), 6);
  }
  if(Parser.date.isValid()) {
        date = twoDigits(Parser.date.day()) + "/" +
               twoDigits(Parser.date.month()) + "/" +
               String(Parser.date.year()) + "      ";
  }
  if(Parser.time.isValid()) {
      time = twoDigits(Parser.time.hour()) + ":" +
             twoDigits(Parser.time.minute()) + ":" +
             twoDigits(Parser.time.second()) + " UTC    ";
  }

  Serial.println(lat);
  Serial.println(lng);
  Serial.println(date);
  Serial.println(time);
  Serial.println();
  
  //if (Serial.available())
    //SIM800.write(Serial.read());
}
// RING: ATD+79047606909;
