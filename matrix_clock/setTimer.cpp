#include "setTimer.h"

const uint64_t longPressDelay = 500;
uint32_t lastDebounceTime = 0;

byte pinSetMode;
byte buttonModeState;
byte buttonModeLastState = HIGH;

byte pinSetUp;
byte buttonUpState;
byte buttonUpLastState = HIGH;

byte pinSetDown;
byte buttonDownState;
byte buttonDownLastState = HIGH;

uint32_t maxDuration = 40;
int32_t duration = 1;

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

        buttonModeState = digitalRead(pinSetMode);
        //When pressed
        if (buttonModeState == LOW && buttonModeState != buttonModeLastState)
        {
            Serial.println("Mode button pressed");

            lastDebounceTime = millis();
            buttonModeLastState = buttonModeState;
        }

        //When released
        if (buttonModeState == HIGH && buttonModeState != buttonModeLastState)
        {
            unsigned long delay = millis() - lastDebounceTime;
            char str[10];

            Serial.println("Mode button up");
            Serial.print(delay);
            Serial.print(" : ");
            Serial.println(sprintf(str, "%u", longPressDelay));

            if (delay > longPressDelay)
            {
                Serial.println("Timer Set exit");
                exit = false;
            }

            buttonModeLastState = buttonModeState;
        }

        buttonUpState = digitalRead(pinSetUp);
        //When pressed
        if (buttonUpState == LOW && buttonUpState != buttonUpLastState)
        {
            Serial.println("Up button pressed");

            lastDebounceTime = millis();
            buttonUpLastState = buttonUpState;
        }

        //When released
        if (buttonUpState == HIGH && buttonUpState != buttonUpLastState)
        {
            unsigned long delay = millis() - lastDebounceTime;
            char str[10];

            Serial.println("Up button released");
            Serial.print(delay);
            Serial.print(" : ");
            Serial.println(sprintf(str, "%u", longPressDelay));

            if (delay > longPressDelay)
            {
                //long press inc by 5
                duration += 5;
            }
            else
            {
                //short press inc by 1
                duration++;
            }

            if (duration > maxDuration)
                duration = maxDuration;

            updateMatrixByValue(duration);
            buttonUpLastState = buttonUpState;
        }

        buttonDownState = digitalRead(pinSetDown);
        //When pressed
        if (buttonDownState == LOW && buttonDownState != buttonDownLastState)
        {
            Serial.println("Down button pressed");

            lastDebounceTime = millis();
            buttonDownLastState = buttonDownState;
        }

        //When released
        if (buttonDownState == HIGH && buttonDownState != buttonDownLastState)
        {
            unsigned long delay = millis() - lastDebounceTime;
            char str[10];

            Serial.println("Down button released");
            Serial.print(delay);
            Serial.print(" : ");
            Serial.println(sprintf(str, "%u", longPressDelay));

            if (delay > longPressDelay)
            {
                //long press dev by 5
                duration -= 5;
            }
            else
            {
                //short press dev by 1
                duration--;
            }

            if (duration < 1)
                duration = 1;

            updateMatrixByValue(duration);
            buttonDownLastState = buttonDownState;
        }
    }

    return duration;
}