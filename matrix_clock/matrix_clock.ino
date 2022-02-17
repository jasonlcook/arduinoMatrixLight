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
//  -   Set Matrix with bit patterns

//Buttons
const byte PIN_LED = 13;

volatile byte modeChange = false;
volatile byte currentMode = 0;

const byte PIN_SET_MODE = 2;
const byte PIN_SET_DOWN = 9;
const byte PIN_SET_UP = 8;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(PIN_LED, OUTPUT);

    //Buttons
    pinMode(PIN_SET_UP, INPUT_PULLUP);
    pinMode(PIN_SET_DOWN, INPUT_PULLUP);

    pinMode(PIN_SET_MODE, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_SET_MODE), setTimerInterrupt, RISING);

    //LED Matrix constructor
    setupMatrix();

    //Countdown timer constructor
    setupCountDown(1);

    //Countdown timer constructor
    setupSettings(PIN_SET_MODE, PIN_SET_UP, PIN_SET_DOWN);
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
    if (modeChange)
    {
        currentMode++;

        if (currentMode > 2)
            currentMode = 0;

        modeChange = false;
    }

    switch (currentMode)
    {
    case 1:
        int32_t duration;

        Serial.println("Curernt mode: Set timer");
        
        duration = setTimer();

        Serial.print("Duration: ");
        Serial.println(duration);

        setupCountDown(duration);

        break;
    case 2:
        Serial.println("Curernt mode: Countdown");
        
        startCountDown();
        break;
    default:
        Serial.println("Curernt mode: Idle");
        
        startIdle();
        break;
    }
}