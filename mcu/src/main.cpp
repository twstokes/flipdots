#include "auth.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FlipDotMatrix.h>

WiFiUDP Udp;
unsigned int localUdpPort = 4210; // local port to listen on
byte incomingPacket[255];         // buffer for incoming packets

FlipDotMatrix matrix = FlipDotMatrix(28, 14, &Serial, 57600);

void setup()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    Udp.begin(localUdpPort);

    matrix.start();
    matrix.setTextColor(1);
    matrix.setTextWrap(false);
}

int x = matrix.width();

void acceptPackets()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        String reply;
        // receive incoming UDP packets
        int len = Udp.read(incomingPacket, 56); // <--- it may be nice to have a big buffer then truncate it
        if (len > 0)
        {
            // this should be 56 from the client
            // 28 1-byte rows, * 2
            incomingPacket[len] = 0; // this terminates payload
        }

        matrix.replaceBoardBuffer(incomingPacket, len);
        matrix.commitAndDisplayBuffer();
    }
}

// driving the display from the MCU
// void loop()
// {
//     matrix.fillScreen(0);
//     matrix.setCursor(x, 4);
//     matrix.print(F("Hi"));

//     matrix.sendBufferToAllPanels(false); // todo - should be hidden
//     // should there be a delay here, or is this sync?
//     matrix.refreshDisplays();

//     if (--x < -60)
//     {
//         x = matrix.width();
//     }

//     delay(50);
// }

// UDP framebuffer mode
void loop()
{
    acceptPackets();
    delay(50);
}