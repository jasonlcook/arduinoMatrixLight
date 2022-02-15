#ifndef LED_MATRIX_FUNCTIONS_H
#define LED_MATRIX_FUNCTIONS_H

#include <MaxMatrix.h>
#include "Arduino.h"

void setupMatrix();
void incrementMatrix();
void decrementMatrix();
void updateMatrix(uint32_t &currentColumn, int32_t &currentRow);
void alarm();
void flashCursor(uint32_t currentColumn, int32_t currentRow);
void clearMatrix();
void fillMatrix();

#endif