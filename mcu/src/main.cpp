#include "auth.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

#include <FlipDotMatrix.h>
#include <Fonts/TomThumb.h>

#define NTP_SERVER "pool.ntp.org"
#define NY_TZ "EST5EDT,M3.2.0,M11.1.0"

time_t now;
tm tm;

FlipDotMatrix matrix = FlipDotMatrix(28, 14, &Serial, 57600);

char buf[3];

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(NY_TZ, NTP_SERVER);

  matrix.start();
  matrix.setTextColor(1);
  matrix.setTextWrap(false);
  matrix.setFont(&TomThumb);
}

// driving the display from the MCU
void loop() {
  time(&now);
  localtime_r(&now, &tm);

  matrix.fillScreen(0);
  matrix.setCursor(6, 9);
  sprintf(buf, "%02d", tm.tm_hour);
  matrix.print(buf);
  matrix.print(":");
  sprintf(buf, "%02d", tm.tm_min);
  matrix.print(buf);
  matrix.commitAndDisplayBufferQuietly(500);

  delay(5000);
}
