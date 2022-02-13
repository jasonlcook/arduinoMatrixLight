#ifndef COUNT_DOWN_FUNCTIONS_H
#define COUNT_DOWN_FUNCTIONS_H

#include <DS3231.h>
#include "ledMatrix.h"
#include "Arduino.h"

void setupCountDown(String minutesDuration);
void countDown();
uint32_t calculateDuration(String input);
uint32_t calculateTick(int duration);

#endif