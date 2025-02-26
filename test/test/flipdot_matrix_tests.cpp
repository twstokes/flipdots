#include <TestFlipDotMatrix.h>
#include <unity.h>

// Note: We purposely don't call matrix.start()
// so that we can test without making a serial connection

// how many bytes in a payload to skip before we
// get to the column (dot value) bytes
#define COLUMN_BYTES_OFFSET 3

void setUp(void) {}

void tearDown(void) {}

// test that the matrix size is computed properly
void test_matrix_size(void) {
  /*
   * layout: 0
   *         1
   */
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 2, 1);
  TEST_ASSERT_EQUAL(28, matrix.width());
  TEST_ASSERT_EQUAL(14, matrix.height());

  /*
   * layout: 0 1
   *         2 3
   */
  TestFlipDotMatrix matrix2 =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 4, 2);
  TEST_ASSERT_EQUAL(56, matrix2.width());
  TEST_ASSERT_EQUAL(14, matrix2.height());

  TestFlipDotMatrix matrix3 =
      TestFlipDotMatrix(FlipDotController::PanelType::p14x7, 4, 2);
  TEST_ASSERT_EQUAL(28, matrix3.width());
  TEST_ASSERT_EQUAL(14, matrix3.height());
}

// test that invalid layouts default to 0x0
void test_matrix_invalid_size(void) {
  // a matrix has to be in a rectangular layout
  // otherwise the library should return zeros for dimensions
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 4, 3);
  TEST_ASSERT_EQUAL(0, matrix.width());
  TEST_ASSERT_EQUAL(0, matrix.height());
}

// test that dimensions are calculated properly
void test_compute_matrix_dimensions(void) {
  TEST_ASSERT_EQUAL(28, FlipDotMatrix::computeMatrixWidth(2, 1, 28));
  TEST_ASSERT_EQUAL(56, FlipDotMatrix::computeMatrixWidth(2, 2, 28));
  TEST_ASSERT_EQUAL(0, FlipDotMatrix::computeMatrixWidth(4, 3, 28));

  TEST_ASSERT_EQUAL(14, FlipDotMatrix::computeMatrixHeight(2, 1));
  TEST_ASSERT_EQUAL(7, FlipDotMatrix::computeMatrixHeight(2, 2));
  TEST_ASSERT_EQUAL(0, FlipDotMatrix::computeMatrixHeight(4, 3));
}

// test setting individual bits in a column
void test_set_bit_in_column(void) {
  uint8_t column = 0;
  FlipDotMatrix::setBitInColumn(&column, 2, 1);
  TEST_ASSERT_EQUAL(4, column);

  // try to set the MSB, which should fail and not change the byte
  FlipDotMatrix::setBitInColumn(&column, 7, 1);
  TEST_ASSERT_EQUAL(4, column);
}

// test the payloads that are sent to the flip dot controller
void test_payload_28x7(void) {
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 2, 1);
  // flip the 1st, 3rd, and 5th dot of the first panel's column
  matrix.drawPixel(0, 0, 1);
  matrix.drawPixel(0, 2, 1);
  matrix.drawPixel(0, 4, 1);
  // flip the dot in the bottom-right corner of the second panel
  matrix.drawPixel(27, 13, 1);
  matrix.show();

  // two panels with 28 columns + four frame bytes
  TEST_ASSERT_EQUAL(32, matrix.payloadSizes[0]);
  TEST_ASSERT_EQUAL(32, matrix.payloadSizes[1]);
  // three total payloads written - two to write to the panels, one to tell the
  // panels to refresh
  TEST_ASSERT_EQUAL(3, matrix.payloadsWritten);
  // the first column of the first panel should have three dots flipped - the
  // 1st, 3rd, and 5th
  TEST_ASSERT_EQUAL(21, matrix.writtenPayloads[0][COLUMN_BYTES_OFFSET]);
  TEST_ASSERT_EQUAL(64, matrix.writtenPayloads[1][27 + COLUMN_BYTES_OFFSET]);
}

void test_payload_7x7(void) {
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p7x7, 2, 1);
  // flip the 1st, 3rd, and 5th dot of the first panel's column
  matrix.drawPixel(0, 0, 1);
  matrix.drawPixel(0, 2, 1);
  matrix.drawPixel(0, 4, 1);
  // flip the dot in the bottom-right corner of the second panel
  matrix.drawPixel(6, 13, 1);
  matrix.show();

  // two panels with 7 columns + four frame bytes
  TEST_ASSERT_EQUAL(11, matrix.payloadSizes[0]);
  TEST_ASSERT_EQUAL(11, matrix.payloadSizes[1]);
  // three total payloads written - two to write to the panels, one to tell the
  // panels to refresh
  TEST_ASSERT_EQUAL(3, matrix.payloadsWritten);
  // the first column of the first panel should have three dots flipped - the
  // 1st, 3rd, and 5th
  TEST_ASSERT_EQUAL(21, matrix.writtenPayloads[0][COLUMN_BYTES_OFFSET]);
  TEST_ASSERT_EQUAL(64, matrix.writtenPayloads[1][6 + COLUMN_BYTES_OFFSET]);
}

// test that filling the screen succeeds
void test_fill_screen(void) {
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 5, 1);
  matrix.show();

  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 28; y++) {
      TEST_ASSERT_EQUAL(0, matrix.writtenPayloads[x][y + COLUMN_BYTES_OFFSET]);
    }
  }

  matrix.resetStats();
  matrix.fillScreen(1);
  matrix.show();

  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 28; y++) {
      // 0x7F == all 1s but the MSB - which has to stay 0
      TEST_ASSERT_EQUAL(0x7F,
                        matrix.writtenPayloads[x][y + COLUMN_BYTES_OFFSET]);
    }
  }
}

// test inverting the screen
void test_inversion(void) {
  TestFlipDotMatrix matrix =
      TestFlipDotMatrix(FlipDotController::PanelType::p28x7, 2, 1);
  // flip the 1st, 3rd, and 5th dot of the first panel's column
  matrix.drawPixel(0, 0, 1);
  matrix.drawPixel(0, 2, 1);
  matrix.drawPixel(0, 4, 1);
  // flip the dot in the bottom-right corner of the second panel
  matrix.drawPixel(27, 13, 1);
  matrix.show();

  // the first column of the first panel should have three dots flipped - the
  // 1st, 3rd, and 5th
  TEST_ASSERT_EQUAL(21, matrix.writtenPayloads[0][COLUMN_BYTES_OFFSET]);
  TEST_ASSERT_EQUAL(64, matrix.writtenPayloads[1][27 + COLUMN_BYTES_OFFSET]);

  // we don't care about the previous stats, we need the memory
  matrix.resetStats();

  // invert the display, which flips the bits in the buffer and calls show()
  // calling show() should equate to three more payloads - two buffers and then
  // the commit call
  matrix.invertDisplay(true);

  // the columns should now be the inverse
  uint8_t expectation1 = (~(uint8_t)21) & 0x7F;
  uint8_t expectation2 = (~(uint8_t)64) & 0x7F;
  TEST_ASSERT_EQUAL(expectation1,
                    matrix.writtenPayloads[0][COLUMN_BYTES_OFFSET]);
  TEST_ASSERT_EQUAL(expectation2,
                    matrix.writtenPayloads[1][27 + COLUMN_BYTES_OFFSET]);

  // now that we're in inversion mode, flipping a dot should be the opposite
  matrix.resetStats();
  // this should fill the screen with all flipped (colored) dots
  matrix.fillScreen(0);
  matrix.show();
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 28; y++) {
      TEST_ASSERT_EQUAL(0x7F,
                        matrix.writtenPayloads[x][y + COLUMN_BYTES_OFFSET]);
    }
  }

  matrix.resetStats();
  // this should flip the top-left dot to black (0) while the others in the
  // column remain flipped (1) - producing a byte with 0 for MSB and LSB (126)
  matrix.drawPixel(0, 0, 1);
  matrix.show();
  TEST_ASSERT_EQUAL(126, matrix.writtenPayloads[0][COLUMN_BYTES_OFFSET]);
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_matrix_size);
  RUN_TEST(test_matrix_invalid_size);
  RUN_TEST(test_payload_28x7);
  RUN_TEST(test_payload_7x7);
  RUN_TEST(test_fill_screen);
  RUN_TEST(test_inversion);
  RUN_TEST(test_compute_matrix_dimensions);
  RUN_TEST(test_set_bit_in_column);
  return UNITY_END();
}

void setup() {
  delay(2000);
  runUnityTests();
}

void loop() {}
