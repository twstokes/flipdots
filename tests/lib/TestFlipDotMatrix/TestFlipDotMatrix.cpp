#include <TestFlipDotMatrix.h>

TestFlipDotMatrix::TestFlipDotMatrix(FlipDotController::PanelType p,
                                     uint8_t panels, uint8_t panelsPerRow)
    : FlipDotMatrix(p, panels, panelsPerRow, &Serial, 57600) {}

void TestFlipDotMatrix::writePayload(const uint8_t *payload, int size) {
  for (int x = 0; x < size; x++) {
    writtenPayloads[payloadsWritten][x] = payload[x];
  }
  payloadSizes[payloadsWritten] = size;
  payloadsWritten++;
}

void TestFlipDotMatrix::resetStats() {
  for (int x = 0; x < MAX_PAYLOADS; x++) {
    payloadSizes[x] = 0;
    for (int y = 0; y < MAX_PAYLOAD_LENGTH; y++) {
      writtenPayloads[x][y] = 0;
    }
  }
  payloadsWritten = 0;
}
