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
uint32_t indexTime;
uint32_t currentTime;
uint32_t secondsSinceLastUpdate;

String minutesDuration;
uint32_t secondDuration;
uint32_t secondsTick;

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

    //buttons
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SET_INTERRUPT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PIN_SET_INTERRUPT), setTimerInterrupt, RISING);

    //Initialise
    calculateDuration("5");
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
        countDown();

    delay(100);
}

void setTimer()
{
    if (debug)
        Serial.println("setTimer()");

    calculateDuration("1");

    settingTimer = false;
}

void countDown()
{
    if (debug)
        Serial.println("countDown()");

    //Read serial value
    while (Serial.available())
    {
        minutesDuration = Serial.readString();
        calculateDuration(minutesDuration);
    }

    //Tick seconds
    DateTime now = myRTC.now();
    currentTime = now.unixtime();

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
        updateMatrix();
        secondsSinceLastUpdate = 0;
    }

    //Flash cursor
    flashCursor();
}

void calculateDuration(String input)
{
    if (debug)
    {
        Serial.print("calculateDuration(");
        Serial.print(input);
        Serial.println(")");
    }

    uint32_t parsedInput = input.toInt();

    secondDuration = parsedInput * 60;

    Serial.print(parsedInput);
    Serial.print(" * 60 ");
    Serial.print(" = ");
    Serial.println(secondDuration);

    secondsTick = calculateTick(secondDuration);

    reset();
}

void reset()
{
    if (debug)
        Serial.println("reset()");

    currentRow = 7;
    currentColumn = 0;

    DateTime now = myRTC.now();
    currentTime = now.unixtime();
    indexTime = currentTime;

    Serial.print("time: ");
    Serial.println(indexTime);

    fill();
}

void fill()
{
    if (debug)
        Serial.println("fill()");

    m.clear();

    for (int i = 0; i < 8; i++)
    {
        m.setColumn(i, B11111111);
    }
}

uint32_t calculateTick(int duration)
{
    if (debug)
    {
        Serial.print("calculateTick(");
        Serial.print(duration);
        Serial.println(")");
    }

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

void updateMatrix()
{
    if (debug)
        Serial.println("updateMatrix()");

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

void flashCursor()
{
    if (debug)
        Serial.println("flashCursor()");

    m.setDot(currentColumn, currentRow, true);
    delay(250);
    m.setDot(currentColumn, currentRow, false);
    delay(250);
}

void alarm()
{
    if (debug)
        Serial.println("alarm()");

    while (true)
    {
        fill();
        delay(250);
        m.clear();
        delay(250);
    }
}
