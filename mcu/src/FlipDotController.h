#include "Arduino.h"
#include <SPI.h>  // added to make PIO compile
#include <Wire.h> // added to make PIO compile

// MAX3485 (3.3v) RS485 -> TTL to MCU connections:
// VCC -> 3.3v
// Gnd -> Gnd
// DE -> 3.3v  pulled high because we're always transmitting
// RE -> 3.3v  pulled high because we're always transmitting
// DI -> TX[x] x being 0 or higher depending on board
// RO -> RX[x] most boards only have the main serial IO,
//             but boards like the Mega have multiple

// baud rate is set via DIP switch on the panel

// see AlfaZeta documentation for other panel types and payload sizes
// commands are based on 28x7 controller
#define FRAME_START 0x80
#define CMD_REFRESH_ALL_DISPLAYS 0x82 // commit buffers on all panels
#define CMD_SEND_AND_REFRESH                                                   \
  0x83 // write buffer to a panel and commit immediately
#define CMD_SEND_NO_REFRESH                                                    \
  0x84 // write buffer to a panel, but don't commit until we send the refresh
       // command (useful for painting all displays at the same time)
#define FRAME_END 0x8F

// max number of data bytes to be sent for 28x7 controller
#define DATA_BYTES 28
// max size of a frame that holds a command for a 28x7 controller
#define CMD_FRAME_SIZE 32

class FlipDotController {
public:
  FlipDotController(HardwareSerial *serial, int baud);
  void start();
  void refreshDisplays();
  void sendBufferToPanel(byte address, const byte *data, bool immediate);

private:
  HardwareSerial *serial;
  const int baud;

  void writePayload(const byte *payload, int size);
};
