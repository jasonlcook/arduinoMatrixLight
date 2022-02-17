#include <Wire.h>

#include "idle.h"
#include "setTimer.h"
#include "countDown.h"

#include "ledMatrix.h"

//todo:
//  -   add alarm buzzer
//  -   add tamper proofing
//  -   add interupt buttons for setting
//  -   add OLED for real output
//  -   user RTC alarm for countdown to allow for poweroutage
//  -   set one minute mode to only use 60 LEDs
//  -   replace delays with clock reads
//  -   simplify button vars between idle and set timer
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

int32_t currentDuration;
int32_t lastDuration;

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

    //Countdown timer constructor
    setupCountDown(1);
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
        unsigned long delay = millis() - lastDebounceTime;
        char str[10];

        Serial.println("Up button released");
        Serial.print(delay);
        Serial.print(" : ");
        Serial.println(sprintf(str, "%u", longPressDelay));

        if (delay > longPressDelay)
            Serial.println("Long press");
        else
            Serial.println("Short press");

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
        unsigned long delay = millis() - lastDebounceTime;
        char str[10];

        Serial.println("Down button released");
        Serial.print(delay);
        Serial.print(" : ");
        Serial.println(sprintf(str, "%u", longPressDelay));

        if (delay > longPressDelay)
            Serial.println("Long press");
        else
            Serial.println("Short press");

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
            break;
        case 2:
            Serial.println("Curernt mode: Countdown");
            break;
        default:
            Serial.println("Curernt mode: Idle");
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
            setupCountDown(currentDuration);
            lastDuration = currentDuration;
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

        startCountDown();
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