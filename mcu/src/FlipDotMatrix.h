#include <Adafruit_GFX.h>
#include <FlipDotController.h>

#define PANEL_COUNT 2
// each 7 dot column is a byte where the MSB isn't used, per the documentation
#define PANEL_COLS 28
// there are 7 dots per column
#define PANEL_ROWS 7

class FlipDotMatrix : public Adafruit_GFX, public FlipDotController {
public:
  FlipDotMatrix(uint8_t matrixW, uint8_t matrixH, HardwareSerial *serial,
                int baud);

  void start();
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillScreen(uint16_t color);
  void replaceBoardBuffer(byte newBuffer[], int bufferSize);
  void sendBufferToAllPanels(bool immediate);
  void commitAndDisplayBuffer();

private:
  /*
      state of all dots on the board(s)
      note that this buffer is a byte per column,
      unlike a byte per "pixel" like most bitmaps use
  */
  byte boardBuffer[PANEL_COUNT][PANEL_COLS];
  const uint8_t matrixWidth, matrixHeight;
};