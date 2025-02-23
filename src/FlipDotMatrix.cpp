#include <FlipDotMatrix.h>

/// @brief                FlipDotMatrix constructor.
/// @param panels         Number of panels in the matrix.
/// @param panelsPerRow   Number of panels per row.
/// @param serial         Serial port to use
/// @param baud           Serial baud rate
FlipDotMatrix::FlipDotMatrix(uint8_t panels, uint8_t panelsPerRow,
                             HardwareSerial *serial, int baud)
    : Adafruit_GFX(computeMatrixWidth(panels, panelsPerRow),
                   computeMatrixHeight(panels, panelsPerRow)),
      FlipDotController(serial, baud), panels(panels) {
  boardBuffer = initializeBuffer();
  deltaBuffer = NULL;
}

FlipDotMatrix::~FlipDotMatrix() {
  free(boardBuffer);
  free(deltaBuffer);
}

/// @brief Start the controller, fill the screen in the unflipped state and
/// present it.
void FlipDotMatrix::start() {
  FlipDotController::start();
  fillScreen(0);
  show();
}

/// @brief Sends the main buffer to all panels.
void FlipDotMatrix::sendBufferToAllPanels() {
  for (uint8_t p = 0; p < panels; p++) {
    sendBufferToPanel(p, boardBuffer);
  }
}

/// @brief            Sends a buffer to a panel.
/// @param p          Panel address (0 through panels - 1)
/// @param buffer     Pointer to a buffer. Defaults to boardBuffer.
/// @param immediate  If the board should present the buffer immediately upon
/// receipt. Defaults to false.
void FlipDotMatrix::sendBufferToPanel(uint8_t p, uint8_t *buffer,
                                      bool immediate) {
  // invalid panel
  if (p >= panels)
    return;

  if (!buffer) {
    buffer = boardBuffer;
  }
  FlipDotController::sendBufferToPanel(p, buffer + (p * PANEL_COLS), immediate);
}

/// @brief        Draws a pixel (dot) to the display using the global matrix
/// coordinates.
/// @param x      X coordinate
/// @param y      Y coordinate
/// @param color  Flipped or unflipped.
void FlipDotMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
    return;
  }

  color = inverted ? !color : color;

  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = WIDTH - 1 - y;
    y = t;
    break;
  case 2:
    x = WIDTH - 1 - x;
    y = HEIGHT - 1 - y;
    break;
  case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }

  int panel = x / PANEL_COLS + (y / PANEL_ROWS) * (WIDTH / PANEL_COLS);
  int col = x % PANEL_COLS;
  int row = y % PANEL_ROWS;

  uint8_t *colPtr = boardBuffer + (panel * PANEL_COLS) + col;
  setBitInColumn(colPtr, row, color);
}

/// @brief        Fills the board buffer with the supplied color.
/// @param color  Flipped or unflipped boolean
void FlipDotMatrix::fillScreen(uint16_t color) {
  // if color is black, show black, else set first 7 bits in byte to 1
  // the MSB should always be 0 per the docs, since only the first 7 bits are
  // used

  // convert to bool
  bool boolColor = color != 0;
  // respect inversion
  boolColor = inverted ? !boolColor : boolColor;
  // write the column
  const uint8_t colValue = boolColor ? 0x7F : 0;

  for (uint8_t col = 0; col < panels * PANEL_COLS; col++) {
    *(boardBuffer + col) = colValue;
  }
}

/// @brief Convenience method to send the buffer to all panels and commit
/// (display) it.
void FlipDotMatrix::show() {
  sendBufferToAllPanels();
  refreshDisplays();
}

/// @brief    Sets the display to an inversion mode where dot flips are
/// opposite. When set to a new value it will update the exiting board buffer
/// and display it immediately.
/// @param i  Boolean to turn the mode on or off.
void FlipDotMatrix::invertDisplay(bool i) {
  if (i == inverted) {
    return;
  }
  inverted = !inverted;
  for (uint8_t col = 0; col < panels * PANEL_COLS; col++) {
    *(boardBuffer + col) = ~*(boardBuffer + col) & 0x7F;
  }
  show();
}

/// @brief  Initializes a buffer capable of storing all matrix values
/// @return A buffer zero-initialized or NULL if memory couldn't be allocated.
uint8_t *FlipDotMatrix::initializeBuffer() {
  uint16_t bytes = panels * PANEL_COLS;
  uint8_t *buffer = (uint8_t *)malloc(bytes);
  if (buffer) {
    memset(buffer, 0, bytes);
  }
  return buffer;
}

/// @brief          Flips individual dots when committing to the display to
/// reduce sound.
/// @param delayMs  Delay in milliseconds per dot flip. This delay is added to
/// any existing required delays, such as the one in
/// FlipDotController::writePayload.
void FlipDotMatrix::showQuietly(int delayMs) {
  if (deltaBuffer == NULL) {
    if (!(deltaBuffer = initializeBuffer()))
      return;
  }

  for (int p = 0; p < panels; p++) {
    for (int c = 0; c < PANEL_COLS; c++) {
      uint8_t *deltaColPtr = deltaBuffer + (p * PANEL_COLS) + c;
      uint8_t *boardColPtr = boardBuffer + (p * PANEL_COLS) + c;

      // if the column byte matches, no need to proceed
      if (*deltaColPtr == *boardColPtr)
        continue;

      for (int r = 0; r < PANEL_ROWS; r++) {
        bool dDot = *deltaColPtr & (1 << r);
        bool bDot = *boardColPtr & (1 << r);

        // if the dot matches, no need to proceed
        if (dDot == bDot)
          continue;

        FlipDotMatrix::setBitInColumn(deltaColPtr, r, bDot);
        sendBufferToPanel(p, deltaBuffer, true);
        delay(delayMs);
      }
    }
  }
}
