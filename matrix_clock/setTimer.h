#ifndef SET_TIMER_FUNCTIONS_H
#define SET_TIMER_FUNCTIONS_H

#include "ledMatrix.h"
#include "Arduino.h"

void setupSettings(byte PIN_SET_MODE, byte PIN_SET_UP, byte PIN_SET_DOWN);
int32_t setTimer();

void setTimerButtonUpShortPress();
void setTimerButtonUpLongPress();
void setTimerButtonDownShortPress();
void setTimerButtonDownLongPress();

#endif