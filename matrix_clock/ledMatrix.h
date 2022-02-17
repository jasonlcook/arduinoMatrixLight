#ifndef LED_MATRIX_FUNCTIONS_H
#define LED_MATRIX_FUNCTIONS_H

#include <MaxMatrix.h>
#include "Arduino.h"

void setupMatrix();
void setMatrix(byte currentColumn, byte currentRow, bool state);
void updateMatrixByValue(int32_t value);
void updateMatrix(byte &currentColumn, int32_t &currentRow, bool state);
void alarm();
void flashCursor(byte currentColumn, int32_t currentRow);
void clearMatrix();
void fillMatrix();

#endif