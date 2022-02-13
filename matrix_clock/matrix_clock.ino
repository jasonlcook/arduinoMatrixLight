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

const byte PIN_SET_INTERRUPT = 2;
const byte PIN_SET_UP = 9;
const byte PIN_SET_DOWN = 10;

void setup()
{
    setupMatrix();

    //Time init
    Serial.begin(9600);
    Wire.begin();

    //Buttons
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SET_INTERRUPT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_SET_INTERRUPT), setTimerInterrupt, RISING);

    //Initialise
    setupCountDown("1");
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
        settingTimer = setTimer();
    else
    {
        //Read serial value
        while (Serial.available())
        {
            String minutesDuration = Serial.readString();
        }

        countDown();
    }
}