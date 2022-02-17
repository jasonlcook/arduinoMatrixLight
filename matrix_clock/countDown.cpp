#include "countDown.h"

uint32_t secondsTick;
uint32_t secondsSinceLastUpdate = 0;
uint32_t indexTime;

byte lightsInMatrix = 64;
byte currentColumn = 0;
int32_t currentRow = 7;

void setupCountDown(int32_t duration)
{
    //Tick seconds
    RTClib myRTC;
    DateTime now = myRTC.now();
    indexTime = now.unixtime();

    secondsTick = calculateDuration(duration);

    fillMatrix();
    currentColumn = 0;
    currentRow = 7;
}

bool startCountDown()
{
    RTClib myRTC;
    DateTime now = myRTC.now();
    uint32_t currentTime = now.unixtime();

    if (currentTime != indexTime)
    {
        secondsSinceLastUpdate++;
        indexTime = currentTime;
    }

    if (secondsSinceLastUpdate >= secondsTick)
    {        
        //make sure the previous dot is off before calculating the current one
        setMatrix(currentColumn, currentRow, false);

        secondsSinceLastUpdate = 0;

        currentRow -= 1;
        if (currentRow < 0)
        {
            currentRow = 7;
            currentColumn += 1;
            if (currentColumn > 7)
            {
                return true;
            }
        }

        setMatrix(currentColumn, currentRow, false);
    }

    return false;
}

void flashCursor(bool state)
{
    setMatrix(currentColumn, currentRow, state);
}

uint32_t calculateDuration(uint32_t input)
{
    uint32_t secondDuration = input * 60;
    return calculateTick(secondDuration);
}

uint32_t calculateTick(int duration)
{
    double tick = (double)duration / (double)lightsInMatrix;
    uint32_t flooredTick = round(tick);

    return flooredTick;
}