#include <FlipDotController.h>

FlipDotController::FlipDotController(HardwareSerial *serial, int baud)
    : serial(serial), baud(baud) {}

/*
    must be called before using the FlipDotController display
*/
void FlipDotController::start() {
  // speed determined by dip switch #2 on controller
  serial->begin(baud);

  while (!serial) {
    ;
  }
}

/*
    writes the frame defined in the AlfaZeta documentation to the board
   controller
*/
void FlipDotController::writePayload(const byte *payload, int size) {
  serial->write(payload, size);
  // a delay is needed so that we can wait for the board to update
  // this delay varies depending on configuration
  delay(50);
}

/*
    tell all controllers to commit their buffers, useful for synchronous drawing
*/
void FlipDotController::refreshDisplays() {
  byte payload[3] = {FRAME_START, CMD_REFRESH_ALL_DISPLAYS, FRAME_END};
  writePayload(payload, 3);
}

/*
    sends bytes to panel at address

    if immediate is true, the panel will write out the buffer as soon as it is
   received if immediate is false, refreshDisplays() should be called eventually
   to commit all buffers
*/
void FlipDotController::sendBufferToPanel(byte address, const byte *data,
                                          bool immediate) {
  byte payload[CMD_FRAME_SIZE];

  payload[0] = FRAME_START;
  payload[1] = immediate ? CMD_SEND_AND_REFRESH : CMD_SEND_NO_REFRESH;
  payload[2] = address;

  for (int c = 0; c < DATA_BYTES; c++) {
    payload[c + 3] = data[c];
  }

  payload[CMD_FRAME_SIZE - 1] = FRAME_END;
  writePayload(payload, CMD_FRAME_SIZE);
}
