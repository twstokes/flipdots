#ifndef FLIPDOTMATRIX_H
#define FLIPDOTMATRIX_H

#include <Adafruit_GFX.h>
#include <FlipDotController.h>

// there are 28 columns per panel
#define PANEL_COLS 28
// there are 7 dots per column
#define PANEL_ROWS 7

class FlipDotMatrix : public Adafruit_GFX, public FlipDotController {
public:
  FlipDotMatrix(uint8_t panels, uint8_t panelsPerRow, HardwareSerial *serial,
                int baud);
  ~FlipDotMatrix();
  void start();
  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void fillScreen(uint16_t color) override;
  void invertDisplay(bool i) override;
  void show();
  void showQuietly(int delayMs);

  /// @brief                Computes the height of the matrix based on number of
  ///                       panels and panels per row.
  /// @param panels         Number of panels for the display.
  /// @param panelsPerRow   Number of panels per row.
  /// @return               Width of the matrix, or 0 if there was an error.
  static uint8_t computeMatrixWidth(uint8_t panels, uint8_t panelsPerRow) {
    // passing panels only to make sure the matrix is a rectangle
    // if not, we return 0
    if (panels % panelsPerRow)
      return 0;

    return panelsPerRow * PANEL_COLS;
  }

  /// @brief                Computes the height of the matrix based on number of
  ///                       panels and panels per row.
  /// @param panels         Number of panels for the display.
  /// @param panelsPerRow   Number of panels per row.
  /// @return               Height of the matrix, or 0 if there was an error.
  static uint8_t computeMatrixHeight(uint8_t panels, uint8_t panelsPerRow) {
    if (!panels || (panels % panelsPerRow))
      return 0;

    return (panels / panelsPerRow) * PANEL_ROWS;
  }

  /// @brief          Takes in a byte representing a column and sets the row to
  ///                 the provided value. Only 7 bits (PANEL_ROWS) are ever
  ///                 populated and the MSB should remain 0, per the docs.
  /// @param colPtr   Pointer to the column
  /// @param rowIdx   Index of the row (the top dot index is 0)
  /// @param val      Boolean value of the dot
  static void setBitInColumn(uint8_t *colPtr, uint8_t rowIdx, bool val) {
    if (rowIdx >= PANEL_ROWS) {
      return;
    }
    if (val) {
      *colPtr |= (1 << rowIdx);
    } else {
      *colPtr &= ~(1 << rowIdx);
    }
  }

private:
  const uint8_t panels;  // How many flip dot panels
  bool inverted = false; // Set when the board is inverted
  uint8_t *boardBuffer,
      *deltaBuffer; // Buffers that store the dot values. Delta buffer may be
                    // NULL if no methods have needed it.
  uint8_t *initializeBuffer();
  void sendBufferToAllPanels();
  void sendBufferToPanel(uint8_t p, uint8_t *buffer = NULL,
                         bool immediate = false);
};

#endif // FLIPDOTMATRIX_H
