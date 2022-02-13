#include <Wire.h>

#include "countDown.h"
#include "setTimer.h"
#include "ledMatrix.h"

//todo:
//  -   fix column wiper bug
//  -   add alarm buzzer
//  -   add tamper proofing
//  -   add interupt buttons for setting
//  -   add OLED for real output
//  -   user RTC alarm for countdown to allow for poweroutage
//  -   set one minute mode to only use 60 LEDs
//  -   replace delays with clock reads

//LED Matix
const byte DIN = 7; // DIN pin of MAX7219 module
const byte CLK = 6; // CLK pin of MAX7219 module
const byte CS = 5;  // CS pin of MAX7219 module
const byte maxInUse = 1;

MaxMatrix matrix(DIN, CS, CLK, maxInUse);
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
    //Time init
    Serial.begin(9600);
    Wire.begin();

    //LED Matix init
    matrix.init();
    matrix.setIntensity(0); // initial led matrix intensity, 0-15

    //Buttons
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SET_INTERRUPT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_SET_INTERRUPT), setTimerInterrupt, RISING);

    //Tick seconds
    RTClib myRTC;
    DateTime now = myRTC.now();
    indexTime = now.unixtime();

    //Initialise
    fillMatrix(matrix);
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
{ //Read serial value
    while (Serial.available())
    {
        String minutesDuration = Serial.readString();
        secondsTick = calculateDuration(lightsInMatrix, minutesDuration);

        fillMatrix(matrix);
        currentColumn = 0;
        currentRow = 7;
    }

    if (settingTimer)
        settingTimer = setTimer();
    else
    {
        bool update = countDown(indexTime, secondsSinceLastUpdate, secondsTick, currentColumn, currentRow);
        if (update)
        {
            updateMatrix(matrix, currentColumn, currentRow);
            delay(500);
        }
        else
        {
            flashCursor(matrix, currentColumn, currentRow);
        }
    }
}