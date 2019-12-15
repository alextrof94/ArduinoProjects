#include <Arduino.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"

const int DELAY = 100;
const int SWITCH_TIME = 5000;

SoftwareSerial gps_serial(5, 4); /* RX, TX */
TinyGPSPlus gps_parser;

void setup()
{
    Serial.begin(9600);
    gps_serial.begin(9600);
}

String twoDigits(int x) {
  if(x < 10) return "0" + String(x);
  else return String(x);
}

int currentModeTime = 0;
bool showLocation = false;

void loop() {
    while(gps_serial.available() > 0) {
        char temp = gps_serial.read();
        //Serial.write(temp);
        gps_parser.encode(temp);
    }

    String lat  = "Unknown         ";
    String lng  = "location        ";
    if(gps_parser.location.isValid()) {
        lat = "Lat: " + String(gps_parser.location.lat(), 6);
        lng = "Lng: " + String(gps_parser.location.lng(), 6);
    }

    String date = "Unknown date    ";
    if(gps_parser.date.isValid()) {
        date = twoDigits(gps_parser.date.day()) + "/" +
               twoDigits(gps_parser.date.month()) + "/" +
               String(gps_parser.date.year()) + "      ";
    }

    String time = "Unknown time    ";
    if(gps_parser.time.isValid()) {
        time = twoDigits(gps_parser.time.hour()) + ":" +
               twoDigits(gps_parser.time.minute()) + ":" +
               twoDigits(gps_parser.time.second()) + " UTC    ";
    }

    if(showLocation) {
        Serial.println("POS:");
        Serial.println(lat);
        Serial.println(lng);
        Serial.println();
    } else { // show date and time
        Serial.println("DATE:");
        Serial.println(date);
        Serial.println(time);
        Serial.println();
    }

    delay(DELAY);
    currentModeTime += DELAY;
    if(currentModeTime >= SWITCH_TIME) {
        showLocation = !showLocation;
        currentModeTime = 0;
    }
}
