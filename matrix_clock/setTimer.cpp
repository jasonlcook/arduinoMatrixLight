#include "setTimer.h"

byte pinSetMode;
byte setModeValue;

byte pinSetUp;
byte setUpValue;

byte pinSetDown;
byte setDownValue;

uint32_t duration = 1;

void setupSettings(byte PIN_SET_MODE, byte PIN_SET_UP, byte PIN_SET_DOWN)
{
    pinSetMode = PIN_SET_MODE;
    pinSetUp = PIN_SET_UP;
    pinSetDown = PIN_SET_DOWN;
}

uint32_t setTimer()
{
    Serial.println("Timer Set");
    clearMatrix();

    bool exit = true;
    while (exit)
    {
        //Read serial value
        while (Serial.available())
        {
            String input = Serial.readString();
            duration = input.toInt();
            exit = false;
        }

        setModeValue = digitalRead(pinSetMode);
        setUpValue = digitalRead(pinSetUp);
        setDownValue = digitalRead(pinSetDown);

        //exit on second button press
        if (!setModeValue)
        {
            Serial.print("setModeValue: ");
            Serial.println(setModeValue);

            exit = false;

            delay(1000);

            Serial.println("Timer Set exit");
        }

        if (!setDownValue)
        {
            duration--;
            if (duration < 1)
                duration = 1;

            updateMatrix(duration);

            Serial.print("duration: ");
            Serial.println(duration);
        }

        if (!setUpValue)
        {
            duration++;
            updateMatrix(duration);

            Serial.print("duration: ");
            Serial.println(duration);
        }
    }

    return duration;
}