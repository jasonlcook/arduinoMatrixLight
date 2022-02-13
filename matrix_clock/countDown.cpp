#include "countDown.h"

uint32_t secondsTick;
uint32_t secondsSinceLastUpdate = 0;
uint32_t indexTime;

uint32_t lightsInMatrix = 64;
uint32_t currentColumn = 0;
int32_t currentRow = 7;

void setupCountDown(String minutesDuration)
{
    //Tick seconds
    RTClib myRTC;
    DateTime now = myRTC.now();
    indexTime = now.unixtime();

    secondsTick = calculateDuration(minutesDuration);

    fillMatrix();
    currentColumn = 0;
    currentRow = 7;
}

void countDown()
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
        updateMatrix(currentColumn, currentRow);
    }

    flashCursor(currentColumn, currentRow);
}

uint32_t calculateDuration(String input)
{
    uint32_t parsedInput = input.toInt();
    uint32_t secondDuration = parsedInput * 60;

    Serial.print(parsedInput);
    Serial.print(" * 60 ");
    Serial.print(" = ");
    Serial.println(secondDuration);

    return calculateTick(secondDuration);
}

uint32_t calculateTick(int duration)
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