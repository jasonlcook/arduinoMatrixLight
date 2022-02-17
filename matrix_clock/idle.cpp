#include "idle.h"

const uint64_t _idle_longPressDelay = 500;
uint32_t _idle_lastDebounceTime = 0;

byte _idle_pinSetMode;
byte _idle_buttonModeState;

byte _idle_pinSetUp;
byte _idle_buttonUpState;
byte _idle_buttonUpLastState = HIGH;

byte _idle_pinSetDown;
byte _idle_buttonDownState;
byte _idle_buttonDownLastState = HIGH;

uint32_t loopSpeedMax = 1000;
uint32_t loopSpeedMin = 100;
uint32_t loopSpeed = 500;

void setupIdle(byte PIN_SET_MODE, byte PIN_SET_UP, byte PIN_SET_DOWN)
{
    _idle_pinSetMode = PIN_SET_MODE;
    _idle_pinSetUp = PIN_SET_UP;
    _idle_pinSetDown = PIN_SET_DOWN;
}

void startIdle()
{
    bool stayInLoop = true;
    while (stayInLoop)
    {
        _idle_buttonModeState = digitalRead(_idle_pinSetMode);
        if (_idle_buttonModeState == LOW)
            stayInLoop = false;

        _idle_buttonUpState = digitalRead(_idle_pinSetUp);
        //When pressed
        if (_idle_buttonUpState == LOW && _idle_buttonUpState != _idle_buttonUpLastState)
        {
            Serial.println("Up button pressed");

            _idle_lastDebounceTime = millis();
            _idle_buttonUpLastState = _idle_buttonUpState;
        }

        //When released
        if (_idle_buttonUpState == HIGH && _idle_buttonUpState != _idle_buttonUpLastState)
        {
            Serial.println("Up button released");

            loopSpeed += 100;
            if (loopSpeed > loopSpeedMax)
                loopSpeed = loopSpeedMax;

            Serial.print("loop speed: ");
            Serial.println(loopSpeed);

            _idle_buttonUpLastState = _idle_buttonUpState;
        }

        _idle_buttonDownState = digitalRead(_idle_pinSetDown);
        //When pressed
        if (_idle_buttonDownState == LOW && _idle_buttonDownState != _idle_buttonDownLastState)
        {
            Serial.println("Down button pressed");

            _idle_lastDebounceTime = millis();
            _idle_buttonDownLastState = _idle_buttonDownState;
        }

        //When released
        if (_idle_buttonDownState == HIGH && _idle_buttonDownState != _idle_buttonDownLastState)
        {
            Serial.println("Down button released");

            loopSpeed -= 100;
            if (loopSpeed < loopSpeedMin)
                loopSpeed = loopSpeedMin;

            Serial.print("loop speed: ");
            Serial.println(loopSpeed);

            _idle_buttonDownLastState = _idle_buttonDownState;
        }

        bool state = random(0, 2);
        byte column = random(0, 8);
        int32_t row = random(0, 8);

        setMatrix(column, row, state);

        delay(loopSpeed);
    }
}