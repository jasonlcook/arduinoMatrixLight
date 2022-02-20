#include <Wire.h>

#include "idle.h"
#include "setTimer.h"
#include "countDown.h"

#include "ledMatrix.h"

//todo:
//  -   allow calculateDuration to be set by serial write
//  -   flash set timer buttons when held
//  -   flash at bounds of idle array

//Buttons
const byte PIN_LED = 13;

const byte PIN_SET_MODE = 2;
const byte PIN_SET_DOWN = 9;
const byte PIN_SET_UP = 8;

const uint64_t longPressDelay = 500;
uint32_t lastDebounceTime = 0;

byte buttonUpState;
byte buttonUpLastState = HIGH;
byte buttonUpShortPress = false;
byte buttonUpLongPress = false;

byte buttonDownState;
byte buttonDownLastState = HIGH;
byte buttonDownShortPress = false;
byte buttonDownLongPress = false;

volatile byte modeChange = false;
byte currentMode = 0;

int32_t currentDuration = 1;
int32_t lastDuration = -1;

bool alarming = false;
const byte alarmDelay = 255;
uint32_t alarmTime = 0;
byte alarmState = 0;

uint32_t elapsedTime = 0;

const int32_t cursorDelay = 750;
uint32_t cursorTime = 0;
bool cursorState = false;

uint32_t idleDelay;
uint32_t idleTime = 0;

//Buzzer
const byte PIN_BUZZER = 11;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(PIN_LED, OUTPUT);

    pinMode(PIN_BUZZER, OUTPUT);

    //Buttons
    pinMode(PIN_SET_MODE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_SET_MODE), setTimerInterrupt, LOW);
    pinMode(PIN_SET_UP, INPUT_PULLUP);
    pinMode(PIN_SET_DOWN, INPUT_PULLUP);

    //LED Matrix setup
    setupMatrix();

    //Idle setup
    setupIdle();
    idleDelay = getLoopSpeed();
}

void setTimerInterrupt()
{
    modeChange = true;
}

void loop()
{
    //Read serial value
    while (Serial.available())
    {
        String input = Serial.readString();
        currentDuration = input.toInt();
        currentMode = 2;

        Serial.print("Current duration: ");
        Serial.println(currentDuration);
    }

    buttonUpState = digitalRead(PIN_SET_UP);
    if (buttonUpState == LOW && buttonUpState != buttonUpLastState)
    {
        lastDebounceTime = millis();
        buttonUpLastState = buttonUpState;
    }

    if (buttonUpState == HIGH && buttonUpState != buttonUpLastState)
    {
        elapsedTime = millis() - lastDebounceTime;
        char str[10];

        if (elapsedTime > longPressDelay)
        {
            buttonUpLongPress = true;
        }
        else
        {
            buttonUpShortPress = true;
        }

        buttonUpLastState = buttonUpState;
    }

    buttonDownState = digitalRead(PIN_SET_DOWN);
    if (buttonDownState == LOW && buttonDownState != buttonDownLastState)
    {
        lastDebounceTime = millis();
        buttonDownLastState = buttonDownState;
    }

    if (buttonDownState == HIGH && buttonDownState != buttonDownLastState)
    {
        elapsedTime = millis() - lastDebounceTime;
        char str[10];

        if (elapsedTime > longPressDelay)
        {
            buttonDownLongPress = true;
        }
        else
        {
            buttonDownShortPress = true;
        }

        buttonDownLastState = buttonDownState;
    }

    if (modeChange)
    {
        currentMode++;

        if (currentMode > 2)
            currentMode = 1;

        switch (currentMode)
        {
        case 1:
            Serial.println("Curernt mode: Set timer");

            clearMatrix();
            updateMatrixByValue(currentDuration);

            break;
        case 2:
            Serial.println("Curernt mode: Countdown");

            fillMatrix();
            cursorTime = millis();

            setupCountDown(currentDuration);

            break;
        case 3:
            Serial.println("Curernt mode: Alarm");
            break;
        default:
            Serial.println("Curernt mode: Idle");

            fillMatrix();
            idleTime = millis();

            break;
        }

        if (alarming)
        {
            alarming = false;
            currentMode = 0;
            noTone(PIN_BUZZER);
        }

        modeChange = false;
    }

    switch (currentMode)
    {
    case 1:
        //Set timer
        currentDuration = setTimer();
        if (currentDuration != lastDuration)
        {
            updateMatrixByValue(currentDuration);
            lastDuration = currentDuration;
        }

        if (buttonDownShortPress)
        {
            setTimerButtonDownShortPress();
            buttonDownShortPress = false;
        }

        if (buttonDownLongPress)
        {
            setTimerButtonDownLongPress();
            buttonDownLongPress = false;
        }

        if (buttonUpShortPress)
        {
            setTimerButtonUpShortPress();
            buttonUpShortPress = false;
        }

        if (buttonUpLongPress)
        {
            setTimerButtonUpLongPress();
            buttonUpLongPress = false;
        }
        break;
    case 2:
        //Countdown
        if (buttonDownShortPress || buttonDownLongPress)
        {
            setTimerButtonDownShortPress();
            buttonDownShortPress = false;
            buttonDownLongPress = false;
        }

        if (buttonUpShortPress || buttonUpLongPress)
        {
            setTimerButtonUpShortPress();
            buttonUpShortPress = false;
            buttonUpLongPress = false;
        }

        elapsedTime = millis() - cursorTime;
        if (elapsedTime > cursorDelay)
        {
            flashCursor(cursorState);

            if (cursorState)
                cursorState = false;
            else
                cursorState = true;

            cursorTime = millis();
        }

        alarming = loopCountDown();
        if (alarming)
        {
            //if the alarm is active then move to Alarm sate
            currentMode = 3;
            alarmTime = millis();
        }

        break;
    case 3:
        //Alarm
        elapsedTime = millis() - alarmTime;
        if (elapsedTime > alarmDelay)
        {
            if (alarmState)
            {
                fillMatrix();
                tone(PIN_BUZZER, 1000);
                alarmState = false;
            }
            else
            {
                clearMatrix();
                noTone(PIN_BUZZER);
                alarmState = true;
            }

            alarmTime = millis();
        }

        if (buttonDownShortPress || buttonDownLongPress || buttonUpShortPress || buttonUpLongPress)
        {
            modeChange = true;

            buttonDownShortPress = false;
            buttonDownLongPress = false;
            buttonUpShortPress = false;
            buttonUpLongPress = false;
        }

        break;
    default:
        //idle
        if (buttonDownShortPress || buttonDownLongPress)
        {
            idleDelay = idleButtonDown();
            buttonDownShortPress = false;
            buttonDownLongPress = false;
        }

        if (buttonUpShortPress || buttonUpShortPress)
        {
            idleDelay = idleButtonUp();
            buttonUpShortPress = false;
            buttonUpLongPress = false;
        }

        elapsedTime = millis() - idleTime;
        if (elapsedTime > idleDelay)
        {
            loopIdle();
            idleTime = millis();
        }
        break;
    }
}
