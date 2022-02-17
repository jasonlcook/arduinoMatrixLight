#ifndef IDLE_FUNCTIONS_H
#define IDLE_FUNCTIONS_H

#include "ledMatrix.h"
#include "Arduino.h"

void setupIdle(byte PIN_SET_MODE, byte PIN_SET_UP, byte PIN_SET_DOWN);
void startIdle();

#endif