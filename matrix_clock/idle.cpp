#include "idle.h"

uint32_t loopSpeedMax = 1000;
uint32_t loopSpeedMin = 100;
uint32_t loopSpeed = 500;

//Down button
void idleButtonDown()
{
    Serial.println("Down button released");

    loopSpeed -= 100;
    if (loopSpeed < loopSpeedMin)
        loopSpeed = loopSpeedMin;

    Serial.print("loop speed: ");
    Serial.println(loopSpeed);
}

//Up button
void idleButtonUp()
{
    Serial.println("Up button released");

    loopSpeed += 100;
    if (loopSpeed > loopSpeedMax)
        loopSpeed = loopSpeedMax;

    Serial.print("loop speed: ");
    Serial.println(loopSpeed);
}

void startIdle()
{
    bool state = random(0, 2);
    byte column = random(0, 8);
    int32_t row = random(0, 8);

    setMatrix(column, row, state);

    delay(loopSpeed);
}