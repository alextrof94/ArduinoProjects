/*
 * HTTP Client POST Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "kab36_Trofimov";  //ENTER YOUR WIFI SETTINGS
const char *password = "qwertpoiuy";

//Web/Server address to read/write from 
const char *host = "https://chkalovc.ru/projects/home/";   

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200);
    Serial.print(".");
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  pinMode(5, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
}

//=======================================================================
//                    Main Program Loop
//=======================================================================

int prevValue = -1;

void loop() {
  
  if (prevValue != !digitalRead(5)) {
    Serial.println("prevValue:");
    Serial.println(prevValue);
    prevValue = !digitalRead(5);
    Serial.println("newValue:");
    Serial.println(prevValue);
    HTTPClient http;    //Declare object of class HTTPClient
  
    String data, link;
  
    //Post Data
    data = "?home_1=" + String(prevValue);
    link = "ya.ru";// "chkalovc.ru/projects/home/" + data;
    Serial.println("link:");
    Serial.println(link);
    
    http.begin(link);     //Specify request destination
    http.addHeader("user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36");    //Specify content-type header
    
    int httpCode = http.GET();            //Send the request
    String payload = http.getString();    
  
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
  
    http.end();  //Close connection
  }
}
//=======================================================================
