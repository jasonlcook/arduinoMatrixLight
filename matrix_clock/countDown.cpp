#include "countDown.h"

double tick;
uint32_t millisecondSinceLastUpdate = 0;
uint32_t indexTime;

byte lightsInMatrix = 64;
byte currentColumn = 0;
int32_t currentRow = 7;

void setupCountDown(int32_t duration)
{
    //Tick seconds
    indexTime = millis();

    tick = calculateDuration(duration);

    fillMatrix();
    currentColumn = 0;
    currentRow = 7;
}

bool loopCountDown()
{
    uint32_t currentTime = millis();
    if ((currentTime - indexTime) >= tick)
    {
        //make sure the previous dot is off before calculating the current one
        setMatrix(currentColumn, currentRow, false);

        millisecondSinceLastUpdate = 0;

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

        indexTime = currentTime;

        setMatrix(currentColumn, currentRow, false);
    }

    return false;
}

void flashCursor(bool state)
{
    setMatrix(currentColumn, currentRow, state);
}

double calculateDuration(uint32_t minutes)
{
    uint32_t seconds = minutes * 60;
    double secondsTick = (double)seconds / (double)lightsInMatrix;
    double millisecondTick = secondsTick * 1000;

    return millisecondTick;
}