#include "idle.h"

bool idleMatrix[64];

const uint16_t loopSpeedMax = 1000;
const uint8_t loopSpeedMin = 0;
const uint32_t loopSpeedStep = 100;
int32_t loopSpeed = 500;

void setupIdle()
{
    byte arraylength = sizeof(idleMatrix);
    for (int i = 0; i < arraylength; i++)
    {
        idleMatrix[i] = 1;
    }

    fillMatrix();
}

int32_t getLoopSpeed()
{
    return loopSpeed;
}

//Down button
int32_t idleButtonDown()
{
    Serial.println("Down button released");

    loopSpeed -= loopSpeedStep;
    if (loopSpeed < loopSpeedMin)
        loopSpeed = loopSpeedMin;

    Serial.print("loop speed: ");
    Serial.println(loopSpeed);

    return loopSpeed;
}

//Up button
int32_t idleButtonUp()
{
    Serial.println("Up button released");

    loopSpeed += loopSpeedStep;
    if (loopSpeed > loopSpeedMax)
        loopSpeed = loopSpeedMax;

    Serial.print("loop speed: ");
    Serial.println(loopSpeed);

    return loopSpeed;
}

void startIdle()
{
    uint8_t column = random(0, 8);
    uint8_t row = random(0, 8);

    int index;
    index = column * row;

    bool state;
    state = idleMatrix[index];

    state = !state;
    idleMatrix[index] = state;

    setMatrix(column, row, state);
}