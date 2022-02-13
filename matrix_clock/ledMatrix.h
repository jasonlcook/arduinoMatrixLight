#ifndef LED_MATRIX_FUNCTIONS_H
#define LED_MATRIX_FUNCTIONS_H

#include <MaxMatrix.h>
#include "Arduino.h"

void updateMatrix(MaxMatrix m, uint32_t &currentColumn, int32_t &currentRow);
void alarm(MaxMatrix m);
void flashCursor(MaxMatrix m, uint32_t currentColumn, int32_t currentRow);
void clearMatrix(MaxMatrix m);
void fillMatrix(MaxMatrix m);

#endif