/*
    Uses an ESP8266 to fetch the current time from
    an NTP server and display it.
*/

#include "auth.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <FlipDotMatrix.h>
#include <Fonts/TomThumb.h>

#define NTP_SERVER "pool.ntp.org"
// New York time zone - change accordingly
#define NY_TZ "EST5EDT,M3.2.0,M11.1.0"

time_t now;
tm tm;

// 28x7 panel type, two panels total, 1 panel per row, 57600 baud rate
FlipDotMatrix matrix = FlipDotMatrix(FlipDotController::PanelType::p28x7, 2, 1, &Serial, 57600);

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

void loop() {
  time(&now);
  localtime_r(&now, &tm);

  matrix.fillScreen(0);
  // adjust to fit the display
  matrix.setCursor(6, 9);
  sprintf(buf, "%02d", tm.tm_hour);
  matrix.print(buf);
  matrix.print(":");
  sprintf(buf, "%02d", tm.tm_min);
  matrix.print(buf);
  // flip individual dots with a delay between each flip
  matrix.showQuietly(500);
  // flip all the dots at once
  // matrix.show();

  delay(5000);
}
