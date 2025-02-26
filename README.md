# flipdotmatrix
[Adafruit GFX-compatible](https://github.com/adafruit/Adafruit-GFX-Library) library for drawing to [Alfa-Zeta flip dot panels](https://flipdots.com/).

https://github.com/twstokes/flipdots/assets/2092798/b6714fcb-35dc-4459-a608-234dd7d56679

#### Related blog posts:
- https://www.tannr.com/2021/03/31/flip-dots/
- https://www.tannr.com/2023/08/06/flip-dots-the-technical-bits/

## Features
- Compatible with boards running Arduino firmware (Arduino UNO, ESP8266, etc.)
- Supports 28x7, 14x7, and 7x7 panels in a grid layout
- Communicates serially to an Alfa-Zeta panel using their protocol
- Supports [Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library) drawing functions like:
  - Graphics primitives (points, lines, circles, rectangles)
  - Text with customizable fonts
  - Canvas rotation
  - Inversion
- Basic tests are implemented (see `test` directory)

## Quickstart
1. Make sure the panels are configured to be addressed in sequential order (see [panel layout requirements](#panel-layout-requirements)).
2. Make sure the baud rate is correctly set on the controller via DIP switch (see [documentation](#alfa-zeta-documentation)).
3. For two 28x7 panels that create a 28x14 (w x h) matrix, code to flip each individual dot in sequential order would look like:

```c
#include "Arduino.h"
#include <FlipDotMatrix.h>

bool flipped = false;

// 28x7 panel type, two panels total, one panel per row, 57600 baud rate
FlipDotMatrix matrix = FlipDotMatrix(FlipDotController::PanelType::p28x7, 2, 1, &Serial, 57600);

void setup() {
  // starts up serial connection, fills the board
  matrix.start(); 
}

void loop() {
  flipped = !flipped;

  // flip each individual dot
  for (uint16_t y = 0; y < matrix.height(); y++) {
    for (uint16_t x = 0; x < matrix.width(); x++) {
      matrix.drawPixel(x, y, flipped);
      // send what's been drawn to the flip dot display
      matrix.show();
      delay(50);
    }
  }
}
```

## Memory considerations

Every column on a panel gets a byte of memory. 7 bits of the byte represent the state of the 7 dots in the column, and the MSB is always zero. To store the matrix in memory, `panels * [number of columns in a panel]` is allocated. For two 28x7 panels this would be **56 bytes total**.

A second buffer of the same size can also be allocated if certain methods need to compare what was drawn previously to what is currently being drawn. If those methods are never called, that buffer is not allocated.

## Panel layout requirements

This library assumes that multiple panels are connected in sequential order, i.e. left to right, top to bottom starting with address `0`.

A four panel layout that's two panels wide would look like:
```
0 1
2 3
```

_Note: If the image renders correctly but needs to be rotated, use the [`setRotation()`](https://github.com/adafruit/Adafruit-GFX-Library/blob/f80daa51c92045b8e99b209451244c600ba87ce6/Adafruit_GFX.h#L50) method._

## Repo

This repo is configured as a library so that it can be consumed by development environments like [Arduino IDE](https://www.arduino.cc/en/software) and [PlatformIO](https://platformio.org/).

- `library.json` - Library metadata
- `include` - Matrix and controller headers
- `src` - Matrix and controller implementation
- `test` - Tests (An ESP8266 has to be connected to run these)
- `examples` - Example sketches.
- `scripts` - Some scripts for compiling, formatting, and testing

## Hardware

The following setup was used to write this library:

- [Alfa-Zeta XY5](https://flipdots.com/en/products-services/flip-dot-boards-xy5/) 14x28 (which is two 7x28 panels chained together)
- [NodeMCU ESP8266](https://en.wikipedia.org/wiki/NodeMCU)
- [3.3v RS-485 to TTL](https://www.amazon.com/gp/product/B07V5LND1T/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
- [ALITOVE AC 100-240V to DC 24V 5A Power Supply](https://www.amazon.com/gp/product/B01GC6VS8I/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1)
- [BINZET DC 12V 24V to 5V 5A](https://www.amazon.com/gp/product/B00J3MHT1E/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) used to step down power to the MCU
  - Unnecessary if you power the MCU independently with a 5V source.

![flip_dots](https://github.com/twstokes/flipdots/assets/2092798/5ceceb91-c845-42d1-8203-d8db8634f0e3)

![flip_dots_wiring](https://github.com/twstokes/flipdots/assets/2092798/0f3b4696-e60a-4845-b948-c952b0bcf868)

**MAX3485 (3.3v) RS485 -> TTL to MCU connections:**

```
VCC -> 3.3v
Gnd -> Gnd
DE -> 3.3v  pulled high because we're always transmitting
RE -> 3.3v  pulled high because we're always transmitting
DI -> TX[x] x being 0 or higher depending on board
RO -> RX[x] most boards only have the main serial IO,
             but boards like the Mega have multiple
```

### Alfa-Zeta documentation

Alfa-Zeta provides documentation for their boards on how to configure the DIP switches on the back for the baud rate, panel addresses, magnetizing time, and a test mode. This documentation can be found in other repos as well as requested from the company (although it may require purchasing a panel first). The same goes for their RS-485 protocol which is mostly implemented in `FlipDotController.cpp`. I'm choosing not to include those documents here because I'm not sure of the company's policy, but again, they are easily found by searching.
