#include <Wire.h>
#include <DS3231.h>
#include <MaxMatrix.h>

//todo:
//  -   add alarm buzzer
//  -   add tamper proofing
//  -   add interupt buttons for setting
//  -   add OLED for real output
//  -   user RTC alarm for countdown to allow for poweroutage
//  -   set one minute mode to only use 60 LEDs
//  -   replace delays with clock reads

const byte debug = true;

const byte PIN_LED = 13;

//Time
RTClib myRTC;

uint32_t secondDuration;

//LED Matix
const byte DIN = 7; // DIN pin of MAX7219 module
const byte CLK = 6; // CLK pin of MAX7219 module
const byte CS = 5;  // CS pin of MAX7219 module
const byte maxInUse = 1;

MaxMatrix m(DIN, CS, CLK, maxInUse);

uint32_t lightsInMatrix = 64;
uint32_t currentColumn;
int32_t currentRow;
bool cursorState = true;

//Buttons
const byte PIN_SET_INTERRUPT = 2;

const byte PIN_SET_UP = 9;
const byte PIN_SET_DOWN = 10;

volatile byte settingTimer = false;

uint32_t secondsTick;
uint32_t secondsSinceLastUpdate = 0;

volatile byte countdown = true;
volatile byte timeset = false;

void setup()
{
    if (debug)
        Serial.println("setup()");

    //Time init
    Serial.begin(9600);
    Wire.begin();

    //LED Matix init
    m.init();
    m.setIntensity(0); // initial led matrix intensity, 0-15

    //Buttons
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SET_INTERRUPT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_SET_INTERRUPT), setTimerInterrupt, RISING);

    //Initialise
    calculateDuration("1");
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
    if (debug)
        Serial.println("mainLoop()");

    if (settingTimer)
        setTimer();
    else
        countDown(secondsSinceLastUpdate, secondsTick, currentColumn, currentRow);

    delay(100);
}

void setTimer()
{
    settingTimer = false;
}

void countDown(uint32_t &secondsSinceLastUpdate, uint32_t &secondsTick, uint32_t &currentColumn, int32_t &currentRow)
{
    uint32_t indexTime;

    //Read serial value
    while (Serial.available())
    {
        String minutesDuration = Serial.readString();
        secondsTick = calculateDuration(minutesDuration);
    }

    //Tick seconds
    DateTime now = myRTC.now();
    uint32_t currentTime = now.unixtime();

    if (currentTime != indexTime)
    {
        secondsSinceLastUpdate++;
        indexTime = currentTime;
    }

    Serial.print(currentTime);
    Serial.print(" : ");
    Serial.print(indexTime);
    Serial.print(" : ");
    Serial.println(secondsSinceLastUpdate);

    if (secondsSinceLastUpdate >= secondsTick)
    {
        updateMatrix(currentColumn, currentRow);
        secondsSinceLastUpdate = 0;
    }

    //Flash cursor
    flashCursor(currentColumn, currentRow);
}

uint32_t calculateDuration(String input)
{
    uint32_t parsedInput = input.toInt();

    secondDuration = parsedInput * 60;

    Serial.print(parsedInput);
    Serial.print(" * 60 ");
    Serial.print(" = ");
    Serial.println(secondDuration);

    return calculateTick(secondDuration);
}

void fill()
{
    m.clear();

    for (int i = 0; i < 8; i++)
    {
        m.setColumn(i, B11111111);
    }
}

uint32_t calculateTick(int duration)
{
    double tick = (double)duration / (double)lightsInMatrix;
    uint32_t flooredTick = round(tick);

    Serial.print(duration);
    Serial.print(" / ");
    Serial.print(lightsInMatrix);
    Serial.print(" = ");
    Serial.print(flooredTick);
    Serial.print(" (");
    Serial.print(tick);
    Serial.println(")");

    return flooredTick;
}

void updateMatrix(uint32_t &currentColumn, int32_t &currentRow)
{
    currentRow -= 1;
    if (currentRow < 0)
    {
        currentRow = 7;
        currentColumn += 1;
        if (currentColumn > 7)
        {
            alarm();
        }
    }

    Serial.print(currentRow);
    Serial.print(" x ");
    Serial.println(currentColumn);

    m.setDot(currentColumn, currentRow, false);
}

void flashCursor(uint32_t currentColumn, int32_t currentRow)
{
    m.setDot(currentColumn, currentRow, true);
    delay(250);
    m.setDot(currentColumn, currentRow, false);
    delay(250);
}

void alarm()
{
    while (true)
    {
        fill();
        delay(250);
        
        m.clear();
        delay(250);
    }
}
