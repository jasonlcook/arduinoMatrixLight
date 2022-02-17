#include "idle.h"

bool idleMatrix[64];

uint32_t loopSpeedMax = 1000;
uint32_t loopSpeedMin = 100;
uint32_t loopSpeed = 500;

void setupIdle()
{
    byte arraylength = sizeof(idleMatrix);
    for (int i = 0; i < arraylength; i++)
    {
        idleMatrix[i] = 1;
    }

    fillMatrix();
}

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
    uint8_t column = random(0, 8);
    uint8_t row = random(0, 8);

    int index;
    index = column * row;

    bool state;
    state = idleMatrix[index];

    state = !state;
    idleMatrix[index] = state;

    setMatrix(column, row, state);

    delay(loopSpeed);
}