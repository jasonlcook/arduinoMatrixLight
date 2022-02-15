#include <Wire.h>

#include "countDown.h"
#include "setTimer.h"
#include "ledMatrix.h"

//todo:
//  -   add alarm buzzer
//  -   add tamper proofing
//  -   add interupt buttons for setting
//  -   add OLED for real output
//  -   user RTC alarm for countdown to allow for poweroutage
//  -   set one minute mode to only use 60 LEDs
//  -   replace delays with clock reads

//Buttons
const byte PIN_LED = 13;

volatile byte settingTimer = false;

const byte PIN_SET_MODE = 2;
const byte PIN_SET_UP = 9;
const byte PIN_SET_DOWN = 10;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(PIN_LED, OUTPUT);

    //Buttons
    pinMode(PIN_SET_UP, OUTPUT);
    pinMode(PIN_SET_DOWN, OUTPUT);

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
    if (!settingTimer)
        settingTimer = true;
    else
        settingTimer = false;
}

void loop()
{
    if (settingTimer)
    {
        uint32_t duration = setTimer();
        setupCountDown(duration);
        settingTimer = false;        
    }
    else
    {
        countDown();
    }
}