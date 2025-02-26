#include <Arduino.h>

#define FRAME_START 0x80
// commit buffers on all panels
#define CMD_REFRESH_ALL_DISPLAYS 0x82
#define FRAME_END 0x8F

class FlipDotController {
public:
  enum class PanelType { p28x7, p14x7, p7x7 };
  // baud rate is set via DIP switch on the panel
  FlipDotController(PanelType p, HardwareSerial *serial, int baud);
  // start the controller
  void start();
  // tell all controllers to commit their buffers, useful for synchronous
  // drawing
  void refreshDisplays();
  // write a buffer to a specific panel
  void sendBufferToPanel(uint8_t address, const uint8_t *data, bool immediate);

protected:
  uint8_t getColumnCount() { return panelColumnCount(panelType); }

private:
  const PanelType panelType;
  HardwareSerial *serial;
  const int baud;

  virtual void writePayload(const uint8_t *payload, int size);

  // command to show the panel buffer immediately
  const uint8_t getRefreshCommand() {
    switch (panelType) {
    case PanelType::p28x7:
      return 0x83;
    case PanelType::p14x7:
      return 0x92;
    case PanelType::p7x7:
      return 0x87;
    default:
      return 0;
    }
  }

  // command to store the data in the buffer but not show it immediately
  // to help with synchronous drawing
  const uint8_t getNoRefreshCommand() {
    switch (panelType) {
    case PanelType::p28x7:
      return 0x84;
    case PanelType::p14x7:
      return 0x93;
    case PanelType::p7x7:
      return 0x88;
    default:
      return 0;
    }
  }

  // how many columns (bytes) in a panel
  const static uint8_t panelColumnCount(PanelType p) {
    switch (p) {
    case PanelType::p28x7:
      return 28;
    case PanelType::p14x7:
      return 14;
    case PanelType::p7x7:
      return 7;
    default:
      return 0;
    }
  }
};
