#include <FlipDotMatrix.h>

// todo - command to invert all dots

FlipDotMatrix::FlipDotMatrix(uint8_t mW, uint8_t mH, HardwareSerial *serial, int baud)
    : Adafruit_GFX(mW, mH), FlipDotController(serial, baud), matrixWidth(mW), matrixHeight(mH)
{
}

void FlipDotMatrix::start()
{
    // start the controller
    FlipDotController::start();
    // initialize the board buffer with all zeros
    fillScreen(0);
    commitAndDisplayBuffer();
}

/*
    required implementation by gfx library for drawing
*/
void FlipDotMatrix::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || x >= matrixWidth ||
        y < 0 || y >= matrixHeight)
    {
        // out of bounds
        return;
    }

    /*    
        panels start with id 0 and are ordered 
        from left to right horizontally by row
        
        four panel layout example:  0 1
                                    2 3
    */
    int panel = x / PANEL_COLS + (y / PANEL_ROWS) * (matrixWidth / PANEL_COLS);
    int col = x % PANEL_COLS;
    int dot = y % PANEL_ROWS;

    if (color == 0)
    {
        // clear the bit
        boardBuffer[panel][col] &= ~(1 << dot);
    }
    else
    {
        // set the bit
        boardBuffer[panel][col] |= (1 << dot);
    }
}

/*
    fills the entire buffer with either 0 or 0x7F

    does not send nor commit the buffer to the panels,
    so a follow-up (e.g. commitAndDisplayBuffer) must be called
*/
void FlipDotMatrix::fillScreen(uint16_t color)
{
    // if color is black, show black, else set first 7 bits in byte to 1
    // the MSB should always be 0 per the docs, since only the first 7 bits are used
    byte dotRowValue = (color == 0) ? 0 : 0x7F;

    for (int p = 0; p < PANEL_COUNT; p++)
    {
        for (int c = 0; c < PANEL_COLS; c++)
        {
            boardBuffer[p][c] = dotRowValue;
        }
    }
}

/*
    sends the latest buffer for each panel
    see FlipDotController::sendBufferToPanel for information on immediate parameter
*/
void FlipDotMatrix::sendBufferToAllPanels(bool immediate)
{
    for (int p = 0; p < PANEL_COUNT; p++)
    {
        sendBufferToPanel(p, boardBuffer[p], immediate);
    }
}

/*
    replace the board buffer with the one supplied
*/
void FlipDotMatrix::replaceBoardBuffer(byte *newBuffer, int bufferSize)
{
    if (bufferSize != PANEL_COUNT * PANEL_COLS)
    {
        return;
    }

    for (int c = 0; c < PANEL_COUNT * PANEL_COLS; c++)
    {
        // mask with 0x7F to ensure the MSB isn't set
        // this isn't known to cause a problem, but the docs say to keep it 0
        boardBuffer[c / PANEL_COLS][c % PANEL_COLS] = newBuffer[c] & 0x7F;
    }
}

/*
    convenience method to push the board buffer to the panels
    and then refresh the display
*/
void FlipDotMatrix::commitAndDisplayBuffer()
{
    sendBufferToAllPanels(false);
    refreshDisplays();
}