#include <Wire.h>
#include <DS3231.h>

#include <MaxMatrix.h>

//todo:
//  -   add alarm buzzer
//  -   add update via bluetooth
//  -   set deletions from top left to bottom right

//Time
RTClib myRTC;
uint32_t indexTime;
uint32_t currentTime;
uint32_t secondsSinceLastUpdate;

String minutesDuration;
uint32_t secondDuration;
uint32_t secondsTick;

//LED Matix
int DIN = 7; // DIN pin of MAX7219 module
int CLK = 6; // CLK pin of MAX7219 module
int CS = 5;  // CS pin of MAX7219 module
int maxInUse = 1;

MaxMatrix m(DIN, CS, CLK, maxInUse);

uint32_t lightsInMatrix = 64;
uint32_t currentColumn = 0;
uint32_t currentRow = 0;
bool cursorState = true;

void setup()
{
    //Time init
    Serial.begin(9600);
    Wire.begin();

    //LED Matix init
    m.init();
    m.setIntensity(0); // initial led matrix intensity, 0-15

    //Initialise
    //set default duration
    calculateDuration("1");
}

void loop()
{
    while (Serial.available())
    {
        minutesDuration = Serial.readString();
        calculateDuration(minutesDuration);
    }

    DateTime now = myRTC.now();
    currentTime = now.unixtime();

    //tick seconds
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

    flashCursor();
}

void calculateDuration(String input)
{
    uint32_t parsedInput = input.toInt();

    secondDuration = parsedInput * 60;

    Serial.print(parsedInput);
    Serial.print(" * 60 ");
    Serial.print(" = ");
    Serial.println(secondDuration);

    secondsTick = calculateTick(secondDuration);

    if (secondsTick < 1)
    {
        secondsTick = 1;
        Serial.println("TICK UPDATED TO 1");
    }

    reset();
}

void reset()
{
    currentRow = 0;
    currentColumn = 0;

    DateTime now = myRTC.now();
    currentTime = now.unixtime();
    indexTime = currentTime;

    Serial.print("time: ");
    Serial.println(indexTime);

    //set LED matrix
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

void updateMatrix()
{
    currentRow += 1;
    if (currentRow > 7)
    {
        currentRow = 0;
        currentColumn += 1;
        if (currentColumn > 7)
        {
            m.clear();
            currentRow = 0;
            currentColumn = 0;
        }
    }

    m.setDot(currentColumn, currentRow, false);
}

void flashCursor()
{
    m.setDot(currentColumn, currentRow, true);
    delay(250);
    m.setDot(currentColumn, currentRow, false);
    delay(250);
}
