#include <FlipDotMatrix.h>

// numbers were chosen that were reasonable
// to support tests but not inflate memory too much
#define MAX_PAYLOADS 5
#define MAX_PAYLOAD_LENGTH 50

class TestFlipDotMatrix: public FlipDotMatrix {

	public:
    TestFlipDotMatrix(uint8_t panels, uint8_t panelsPerRow);
    // 2d array to keep a history of the written payloads
    uint8_t writtenPayloads[MAX_PAYLOADS][MAX_PAYLOAD_LENGTH];
    // array to track the size of payloads
    int payloadSizes[MAX_PAYLOADS];
    // total number of payloads written
    int payloadsWritten = 0;
    void resetStats();

	private:
    // overrides the FlipDotController to catch what would be sent over serial
    void writePayload(const uint8_t *payload, int size) override;

};
