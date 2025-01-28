
void setBitInColumn(unsigned char *col, unsigned char row, bool val) {
  if (val) {
    *col |= (1 << row);
  } else {
    *col &= ~(1 << row);
  }
}
