#include <Arduino.h>

// see AlfaZeta documentation for other panel types and payload sizes
// commands are based on 28x7 controller
#define FRAME_START 0x80
// commit buffers on all panels
#define CMD_REFRESH_ALL_DISPLAYS 0x82
// write buffer to a panel and commit immediately
#define CMD_SEND_AND_REFRESH 0x83
// write buffer to a panel, but don't commit until we send the refresh
#define CMD_SEND_NO_REFRESH 0x84
// command (useful for painting all displays at the same time)
#define FRAME_END 0x8F

// max number of data bytes to be sent for a 28x7 panel
#define DATA_BYTES 28
// max size of a frame that holds a command for a 28x7 controller
#define CMD_FRAME_SIZE 32

class FlipDotController {
public:
  // baud rate is set via DIP switch on the panel
  FlipDotController(HardwareSerial *serial, int baud);
  // start the controller
  void start();
  // tell all controllers to commit their buffers, useful for synchronous
  // drawing
  void refreshDisplays();
  // write a buffer to a specific panel
  void sendBufferToPanel(uint8_t address, const uint8_t *data, bool immediate);

private:
  HardwareSerial *serial;
  const int baud;

  virtual void writePayload(const uint8_t *payload, int size);
};
