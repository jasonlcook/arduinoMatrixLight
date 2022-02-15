#include "setTimer.h"

byte pinSetMode;
byte setModeValue;

byte pinSetUp;
byte setUpValue;

byte pinSetDown;
byte setDownValue;

void setupSettings(byte PIN_SET_MODE, byte PIN_SET_UP, byte PIN_SET_DOWN)
{
    pinSetMode = PIN_SET_MODE;
    pinSetUp = PIN_SET_UP;
    pinSetDown = PIN_SET_DOWN;
}

uint32_t setTimer()
{
    Serial.println("Timer Set");

    uint32_t duration;
    bool exit = true;
    while (exit)
    {
        //Read serial value
        while (Serial.available())
        {
            String input = Serial.readString();
            duration = input.toInt();
        }

        setModeValue = digitalRead(pinSetMode);
        setUpValue = digitalRead(pinSetUp);
        setDownValue = digitalRead(pinSetDown);

        //exit on second button press
        if (!setModeValue)
            exit = false;

        Serial.print("setModeValue: ");
        Serial.println(setModeValue);

        Serial.print("setUpValue: ");
        Serial.println(setUpValue);

        Serial.print("setDownValue: ");
        Serial.println(setDownValue);

        Serial.print("exit: ");
        Serial.println(exit);
    }

    return duration;
}