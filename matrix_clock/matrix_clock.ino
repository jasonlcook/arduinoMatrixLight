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

uint32_t lightsInMatrix = 64;

//Buttons
const byte PIN_LED = 13;

volatile byte settingTimer = false;

const byte PIN_SET_INTERRUPT = 2;
const byte PIN_SET_UP = 9;
const byte PIN_SET_DOWN = 10;

uint32_t secondsTick;
uint32_t secondsSinceLastUpdate = 0;
uint32_t indexTime;
uint32_t currentColumn = 0;
int32_t currentRow = 7;

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

    //Tick seconds
    RTClib myRTC;
    DateTime now = myRTC.now();
    indexTime = now.unixtime();

    //Initialise
    fillMatrix();
    secondsTick = calculateDuration(lightsInMatrix, "1");
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
            secondsTick = calculateDuration(lightsInMatrix, minutesDuration);

            fillMatrix();
            currentColumn = 0;
            currentRow = 7;
        }

        bool update = countDown(indexTime, secondsSinceLastUpdate, secondsTick, currentColumn, currentRow);
        if (update)
        {
            updateMatrix(currentColumn, currentRow);
        }
        else
        {
            flashCursor(currentColumn, currentRow);
        }
    }
}