#include <Wire.h>

#include "idle.h"
#include "setTimer.h"
#include "countDown.h"

#include "ledMatrix.h"

//todo:
//  -   add alarm buzzer
//  -   add tamper proofing
//  -   add OLED for real output
//  -   user RTC alarm for countdown to allow for poweroutage
//  -   set one minute mode to only use 60 LEDs
//  -   replace delays with clock reads
//  -   make idle animation toggle light rather then arbitrarily set its state

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

const byte cursorDelay = 100;
uint32_t cursorTime = 0;
bool cursorState = false;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(PIN_LED, OUTPUT);

    //Buttons
    pinMode(PIN_SET_MODE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_SET_MODE), setTimerInterrupt, RISING);

    pinMode(PIN_SET_UP, INPUT_PULLUP);
    pinMode(PIN_SET_DOWN, INPUT_PULLUP);

    //LED Matrix constructor
    setupMatrix();

    fillMatrix();
}

void setTimerInterrupt()
{
    if (!modeChange)
        modeChange = true;
    else
        modeChange = false;
}

void loop()
{
    //Read serial value
    while (Serial.available())
    {
        String input = Serial.readString();
        int32_t duration = input.toInt();
    }

    buttonUpState = digitalRead(PIN_SET_UP);
    if (buttonUpState == LOW && buttonUpState != buttonUpLastState)
    {
        Serial.println("Up button pressed");

        lastDebounceTime = millis();
        buttonUpLastState = buttonUpState;
    }

    if (buttonUpState == HIGH && buttonUpState != buttonUpLastState)
    {
        elapsedTime = millis() - lastDebounceTime;
        char str[10];

        if (elapsedTime > longPressDelay)
        {
            Serial.println("Long press");
            buttonUpLongPress = true;
        }
        else
        {
            Serial.println("Short press");
            buttonUpShortPress = true;
        }

        buttonUpLastState = buttonUpState;
    }

    buttonDownState = digitalRead(PIN_SET_DOWN);
    if (buttonDownState == LOW && buttonDownState != buttonDownLastState)
    {
        Serial.println("Down button pressed");

        lastDebounceTime = millis();
        buttonDownLastState = buttonDownState;
    }

    if (buttonDownState == HIGH && buttonDownState != buttonDownLastState)
    {
        elapsedTime = millis() - lastDebounceTime;
        char str[10];

        if (elapsedTime > longPressDelay)
        {
            Serial.println("Long press");
            buttonDownLongPress = true;
        }
        else
        {
            Serial.println("Short press");
            buttonDownShortPress = true;
        }

        buttonDownLastState = buttonDownState;
    }

    if (modeChange)
    {
        currentMode++;

        if (currentMode > 2)
            currentMode = 0;

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

            break;
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
            Serial.print("currentDuration: ");
            Serial.println(currentDuration);

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

        alarming = startCountDown();
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
                alarmState = false;
            }
            else
            {
                clearMatrix();
                alarmState = true;
            }

            alarmTime = millis();
        }
        break;
    default:
        //idle
        if (buttonDownShortPress)
        {
            idleButtonDown();
            buttonDownShortPress = false;
        }

        if (buttonDownLongPress)
        {
            idleButtonDown();
            buttonDownLongPress = false;
        }

        if (buttonUpShortPress)
        {
            idleButtonUp();
            buttonUpShortPress = false;
        }

        if (buttonUpShortPress)
        {
            idleButtonUp();
            buttonUpLongPress = false;
        }

        startIdle();
        break;
    }
}
