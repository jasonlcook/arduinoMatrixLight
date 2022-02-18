#include "idle.h"

bool idleMatrix[64];

const uint16_t loopSpeedMax = 1000;
const uint8_t loopSpeedMin = 0;
const uint32_t loopSpeedStep = 100;

int loopSpeeds[] = {0, 10, 20, 40, 80, 160, 320, 640};
int32_t loopSpeedIndex = 3;

int32_t loopSpeed;

void setupIdle()
{
    byte arraylength;
    arraylength = sizeof(idleMatrix);
    for (int i = 0; i < arraylength; i++)
    {
        idleMatrix[i] = 1;
    }

    fillMatrix();

    loopSpeed = getLoopSpeed();
}

int32_t getLoopSpeed()
{
    return loopSpeeds[loopSpeedIndex];
}

//Down button
int32_t idleButtonDown()
{
    Serial.println("Down button released");

    loopSpeedIndex--;

    if (loopSpeedIndex < 0)
        loopSpeedIndex = 0;

    loopSpeed = getLoopSpeed();

    return loopSpeed;
}

//Up button
int32_t idleButtonUp()
{
    Serial.println("Up button released");

    byte arraylength;
    arraylength = (sizeof(loopSpeeds) / sizeof(*loopSpeeds)) - 1;

    loopSpeedIndex++;
    if (loopSpeedIndex > arraylength)
        loopSpeedIndex = arraylength;

    loopSpeed = getLoopSpeed();

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