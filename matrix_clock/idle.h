#ifndef IDLE_FUNCTIONS_H
#define IDLE_FUNCTIONS_H

#include "ledMatrix.h"
#include "Arduino.h"

void setupIdle();

int32_t getLoopSpeed();

int32_t idleButtonDown();
int32_t idleButtonUp();

void startIdle();

#endif