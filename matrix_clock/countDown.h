#ifndef COUNT_DOWN_FUNCTIONS_H
#define COUNT_DOWN_FUNCTIONS_H

#include "ledMatrix.h"
#include "Arduino.h"

void setupCountDown(int32_t duration);
bool loopCountDown();
void flashCursor(bool state);
double calculateDuration(uint32_t input);

#endif