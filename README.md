# flipdots
Driving Alfa-Zeta Flip Dots with an ESP8266



https://github.com/twstokes/flipdots/assets/2092798/e8f1af45-21ba-4c02-af97-81e1b29c22db



This repo contains MCU code for driving dots on a 14x28 Alfa-Zeta XY5 display from [my blog post here](https://www.tannr.com/2021/03/31/flip-dots/). It's an experimental hack, but could serve as starting point for others wanted to do similar projects. It could easily be tweaked to handle larger or smaller panels.

Features:
- Communicates via RS-485 to an Alfa-Zeta panel using their protocol
- Listens for UDP packets containing a full screen buffer to draw to the board
- Can draw locally with [Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library)

Nice-to-haves that currently aren't implemented:
- Sending delta changes (the full frame buffer is sent, which is inefficient in some cases)
- Sending commands to run routines independently of an active client (e.g. send a command to scroll a string and then the client disconnects)

## Repo

`/mcu` - The main [PlatformIO](https://platformio.org/) project. The project is configured for a NodeMCU ESP8266 but any compatible hardware can be substituted. The Arduino framework is used.
- `FlipDotController.h` - The Alfa-Zeta protocol implementation to communicate with the board over serial / RS-485.
- `FlipDotMatrix.h` - Integration with the Adafruit GFX library.
- `main.cpp` - The main driver of the MCU, demonstrating a UDP packet mode and drawing locally.

Build the firmware with `run pio` and follow the PlatformIO docs to upload it to a device.

`/tools` - Various tools that helped with testing, mostly written using [Processing](https://processing.org/).
