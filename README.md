# flipdots
Driving Alfa-Zeta Flip Dots with an ESP8266

https://github.com/twstokes/flipdots/assets/2092798/b6714fcb-35dc-4459-a608-234dd7d56679

This repo contains MCU code for driving dots on a 14x28 Alfa-Zeta XY5 display from [my blog post here](https://www.tannr.com/2021/03/31/flip-dots/). It's an experimental hack, but could serve as starting point for others wanted to do similar projects. It could easily be tweaked to handle larger or smaller panels. See [flipdots-ios](https://github.com/twstokes/flipdots-ios/) for the iOS / iPadOS / macOS client app.

**Features:**
- Communicates via RS-485 to an Alfa-Zeta panel using their protocol
- Listens for UDP packets containing a full screen buffer to draw to the board
- Can draw locally with [Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library)

**Nice-to-haves that currently aren't implemented:**
- Sending delta changes (the full frame buffer is sent, which is inefficient in some cases)
- Sending commands to run routines independently of an active client (e.g. send a command to scroll a string and then the client disconnects)

## Repo

`/mcu` - The main [PlatformIO](https://platformio.org/) project. The project is configured for a NodeMCU ESP8266 but any compatible hardware can be substituted. The Arduino framework is used.
- `FlipDotController.h` - The Alfa-Zeta protocol implementation to communicate with the board over serial / RS-485.
- `FlipDotMatrix.h` - Integration with the Adafruit GFX library.
- `main.cpp` - The main driver of the MCU, demonstrating a UDP packet mode and drawing locally.

Build the firmware with `run pio` and follow the PlatformIO docs to upload it to a device.

`/tools` - Various tools that helped with testing, mostly written using [Processing](https://processing.org/).

## Hardware

- [Alfa-Zeta XY5](https://flipdots.com/en/products-services/flip-dot-boards-xy5/) 14x28 (which is two 7x28 panels chained together)
- [NodeMCU ESP8266](https://en.wikipedia.org/wiki/NodeMCU)
- [3.3v RS-485 to TTL](https://www.amazon.com/gp/product/B07V5LND1T/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) see `FlipDotController.h` for pinout
- [ALITOVE AC 100-240V to DC 24V 5A Power Supply](https://www.amazon.com/gp/product/B01GC6VS8I/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1)
- [BINZET DC 12V 24V to 5V 5A](https://www.amazon.com/gp/product/B00J3MHT1E/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) used to step down power to the MCU

![flip_dots](https://github.com/twstokes/flipdots/assets/2092798/5ceceb91-c845-42d1-8203-d8db8634f0e3)

![flip_dots_wiring](https://github.com/twstokes/flipdots/assets/2092798/0f3b4696-e60a-4845-b948-c952b0bcf868)

Alfa-Zeta provides documentation for their boards on how to configure the DIP switches on the back for the baud rate, panel addresses, magnetizing time, and a test mode. This documentation can be found in other repos as well as requested from the company (although it may require purchasing a panel first). The same goes for their RS-485 protocol which is really simple and mostly implemented in `FlipDotController.h`. I'm choosing not to include those documents in this repo because I'm not sure of the company's policy, but again, they are easily found by searching.

The 24V -> 5V converter isn't necessary if you power the MCU independently with a 5V source, I just wanted a single power supply. I've since put the entire project behind a smart switch so I can say "Hey Siri, turn the flip dots on" and they come to life.
