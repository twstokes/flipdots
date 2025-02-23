#include "Arduino.h"
#include <FlipDotMatrix.h>

// two panels total, 1 panel per row, 57600 baud rate
FlipDotMatrix matrix = FlipDotMatrix(2, 1, &Serial, 57600);

// flap the entire display
void flapper() {
  for (int x = 0; x <= 10; x++) {
    matrix.fillScreen(0);
    matrix.show();
    delay(10);
    matrix.fillScreen(1);
    matrix.show();
  }
}


// flip rows of dots on the display from top to bottom
void sequential_dots() {
  matrix.fillScreen(0);
  matrix.show();

  for (uint16_t y = 0; y < matrix.height(); y++) {
    for (uint16_t x = 0; x < matrix.width(); x++) {
      matrix.drawPixel(x, y, 1);
    }
    // send what's been drawn to the flip dot display
    matrix.show();
    delay(100);
 }
}

void setup() {
  // starts up serial connection, fills the board
  matrix.start(); 
}

void loop() {
  flapper();
  sequential_dots();
}

