#ifndef COUNT_DOWN_FUNCTIONS_H
#define COUNT_DOWN_FUNCTIONS_H

#include <DS3231.h>
#include "ledMatrix.h"
#include "Arduino.h"

void setupCountDown(int32_t duration);
void startCountDown();
uint32_t calculateDuration(uint32_t input);
uint32_t calculateTick(int duration);

#endif