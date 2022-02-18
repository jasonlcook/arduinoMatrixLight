#include "setTimer.h"

const byte minDuration = 1;
const byte maxDuration = 40;

int32_t timerDuration = 1;

//Down button
//  Pressed short
void setTimerButtonDownShortPress()
{
    timerDuration--;
}

//  Pressed long
void setTimerButtonDownLongPress()
{
    byte leftOver = timerDuration % 5;

    if (leftOver > 0)
        timerDuration = timerDuration - leftOver;
    else
        timerDuration -= 5;
}

//Up button
//  Pressed short
void setTimerButtonUpShortPress()
{
    timerDuration++;
}

//  Pressed long
void setTimerButtonUpLongPress()
{
    byte leftOver = timerDuration % 5;
    timerDuration = timerDuration - leftOver;

    timerDuration += 5;
}

int32_t setTimer()
{
    if (timerDuration < minDuration)
        timerDuration = minDuration;

    if (timerDuration > maxDuration)
        timerDuration = maxDuration;

    return timerDuration;
}
