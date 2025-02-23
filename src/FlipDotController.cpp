#include <FlipDotController.h>

FlipDotController::FlipDotController(HardwareSerial *serial, int baud)
    : serial(serial), baud(baud) {}

/// @brief Starts communication with the flip dot controller. Must be called
///        before using the display.
void FlipDotController::start() {
  // speed determined by dip switch #2 on controller
  serial->begin(baud);

  while (!serial) {
    ;
  }
}

/// @brief          Writes a frame to the Alfa-Zeta board.
/// @param payload  Pointer to the payload.
/// @param size     Number of bytes in the payload.
void FlipDotController::writePayload(const uint8_t *payload, int size) {
  serial->write(payload, size);
  // a delay is needed so that we can wait for the board to update
  // this delay varies depending on configuration
  delay(50);
}

/// @brief Tells all panels to commit their buffers. Useful for synchronous
///        drawing.
void FlipDotController::refreshDisplays() {
  uint8_t payload[3] = {FRAME_START, CMD_REFRESH_ALL_DISPLAYS, FRAME_END};
  writePayload(payload, 3);
}

/// @brief            Sends bytes to a panel at the given address.
/// @param address    Numeric ID set on the DIP switch of the panel.
/// @param data       Byte array.
/// @param immediate  Set to true if the panel should immediately commit its
///                   buffer upon receipt of the data. If false,
///                   refreshDisplays() should be called eventually to commit
///                   all buffers.
void FlipDotController::sendBufferToPanel(uint8_t address, const uint8_t *data,
                                          bool immediate) {
  uint8_t payload[CMD_FRAME_SIZE];

  payload[0] = FRAME_START;
  payload[1] = immediate ? CMD_SEND_AND_REFRESH : CMD_SEND_NO_REFRESH;
  payload[2] = address;

  for (int c = 0; c < DATA_BYTES; c++) {
    payload[c + 3] = data[c];
  }

  payload[CMD_FRAME_SIZE - 1] = FRAME_END;
  writePayload(payload, CMD_FRAME_SIZE);
}
