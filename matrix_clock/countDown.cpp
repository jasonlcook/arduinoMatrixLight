#include "countDown.h"

bool countDown(uint32_t &indexTime, uint32_t &secondsSinceLastUpdate, uint32_t &secondsTick, uint32_t &currentColumn, int32_t &currentRow)
{
    RTClib myRTC;
    DateTime now = myRTC.now();
    uint32_t currentTime = now.unixtime();

    if (currentTime != indexTime)
    {
        secondsSinceLastUpdate++;
        indexTime = currentTime;
    }

    Serial.print(currentTime);
    Serial.print(" : ");
    Serial.print(indexTime);
    Serial.print(" : ");
    Serial.println(secondsSinceLastUpdate);

    if (secondsSinceLastUpdate >= secondsTick)
    {
        secondsSinceLastUpdate = 0;
        return true;
    }

    return false;
}

uint32_t calculateDuration(uint32_t lightsInMatrix, String input)
{
    uint32_t parsedInput = input.toInt();
    uint32_t secondDuration = parsedInput * 60;

    Serial.print(parsedInput);
    Serial.print(" * 60 ");
    Serial.print(" = ");
    Serial.println(secondDuration);

    return calculateTick(lightsInMatrix, secondDuration);
}

uint32_t calculateTick(uint32_t lightsInMatrix, int duration)
{
    double tick = (double)duration / (double)lightsInMatrix;
    uint32_t flooredTick = round(tick);

    Serial.print(duration);
    Serial.print(" / ");
    Serial.print(lightsInMatrix);
    Serial.print(" = ");
    Serial.print(flooredTick);
    Serial.print(" (");
    Serial.print(tick);
    Serial.println(")");

    return flooredTick;
}