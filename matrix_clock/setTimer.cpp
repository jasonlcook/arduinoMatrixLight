#include "setTimer.h"

const byte minDuration = 1;
const byte maxDuration = 40;

int32_t timerDuration = 1;

//Down button
//  Pressed short
void setTimerButtonDownShortPress()
{
    Serial.println("Timer - Down short button pressed");
    timerDuration--;
    
    Serial.print("Timer duration: ");
    Serial.println(timerDuration);
}

//  Pressed long
void setTimerButtonDownLongPress()
{
    Serial.println("Timer - Down long button pressed");
    timerDuration -= 5;
    
    Serial.print("Timer duration: ");
    Serial.println(timerDuration);
}

//Up button
//  Pressed short
void setTimerButtonUpShortPress()
{
    Serial.println("Timer - Up short button pressed");
    timerDuration++;
    
    Serial.print("Timer duration: ");
    Serial.println(timerDuration);
}

//  Pressed long
void setTimerButtonUpLongPress()
{
    Serial.println("Timer - Up long button pressed");
    timerDuration += 5;
    
    Serial.print("Timer duration: ");
    Serial.println(timerDuration);
}

int32_t setTimer()
{

    if (timerDuration < minDuration)
        timerDuration = minDuration;

    if (timerDuration > maxDuration)
        timerDuration = maxDuration;

    return timerDuration;
}
