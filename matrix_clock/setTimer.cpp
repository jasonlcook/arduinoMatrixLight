#include "setTimer.h"

uint32_t maxDuration = 40;
uint32_t minDuration = 1;

int32_t duration = 1;

//Down button
//  Pressed short
void setTimerButtonDownShortPress()
{
    Serial.println("Down short button pressed");
    duration--;
}

//  Pressed long
void setTimerButtonDownLongPress()
{
    Serial.println("Down long button pressed");
    duration -= 5;
}

//Up button
//  Pressed short
void setTimerButtonUpShortPress()
{
    Serial.println("Up short button pressed");
    duration++;
}

//  Pressed long
void setTimerButtonUpLongPress()
{
    Serial.println("Up long button pressed");
    duration += 5;
}

int32_t setTimer()
{
    if (duration < minDuration)
        duration = minDuration;

    if (duration > maxDuration)
        duration = maxDuration;

    return duration;
}
