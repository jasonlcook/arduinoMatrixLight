#ifndef COUNT_DOWN_FUNCTIONS_H
#define COUNT_DOWN_FUNCTIONS_H

#include <DS3231.h>
#include "Arduino.h"

bool countDown(uint32_t &indexTime, uint32_t &secondsSinceLastUpdate, uint32_t &secondsTick, uint32_t &currentColumn, int32_t &currentRow);
uint32_t calculateDuration(uint32_t lightsInMatrix, String input);
uint32_t calculateTick(uint32_t lightsInMatrix, int duration);

#endif