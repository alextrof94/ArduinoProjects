#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

const uint8_t fingerprint[20] = {0xdd,0x29,0x48,0x01,0x1d,0xba,0xd4,0xd0,0xbb,0xfc,0x7e,0x78,0xca,0x9c,0x77,0x96,0xf1,0xdb,0x62,0x3e};

ESP8266WiFiMulti WiFiMulti;

void setup() {
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  pinMode(5, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  
  Serial.begin(115200);

  Serial.println("Try to connect...");
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("kab36_Trofimov", "qwertpoiuy");

  delay(1000);
}

int prevValue = -1;
uint32_t timer;

void loop() {
  if (prevValue != !digitalRead(5) || millis() > timer) {
    timer += 5000;
    int adcValue=analogRead(A0);
    Serial.println(adcValue);
    Serial.println("prevValue:");
    Serial.println(prevValue);
    prevValue = !digitalRead(5);
    Serial.println("newValue:");
    Serial.println(prevValue);
    if (WiFiMulti.run() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setFingerprint(fingerprint);
      HTTPClient https;
      String data, link;
      data = "?enter=" + String(prevValue) + "&gas=" + String(adcValue);
      link = "https://chkalovc.ru/projects/home/" + data;
      Serial.println(link);
      
      if (https.begin(*client, link)) {
        https.addHeader("user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36");
        int httpCode = https.GET();
        if (httpCode > 0) {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    } else {
      Serial.println("Wait 10s before next round...");
      delay(10000);
    }
  }
}
