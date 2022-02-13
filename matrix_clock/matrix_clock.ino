#include <Wire.h>
#include <DS3231.h>

#include <MaxMatrix.h>

//todo:
//  -   allow the input and updating of the duration
//  -   invert led matrix to count down instead of up
//  -   set the current digit to blink every seccond

//Time
RTClib myRTC;
uint32_t indexTime;
uint32_t currentTime;
uint32_t secondsSinceLastUpdate;

String duration;
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

void setup()
{
    //Time init
    Serial.begin(9600);
    Wire.begin();

    //LED Matix init
    m.init();          // MAX7219 initialization
    m.setIntensity(0); // initial led matrix intensity, 0-15

    //set index time
    DateTime now = myRTC.now();
    currentTime = now.unixtime();
    indexTime = currentTime;

    Serial.print("indexTime: ");
    Serial.println(indexTime);

    Serial.print("currentTime: ");
    Serial.println(currentTime);

    //set default duration
    secondDuration = 64;
    secondsTick = calculateTick(secondDuration);

    Serial.print("secondDuration: ");
    Serial.println(secondDuration);

    Serial.print("secondsTick: ");
    Serial.println(secondsTick);

    //set LED matrix
    m.setDot(0, 0, true);
}

void loop()
{ 
    DateTime now = myRTC.now();
    currentTime = now.unixtime();

    //tick seconds
    if (currentTime != indexTime)
    {
        secondsSinceLastUpdate++;
        indexTime = currentTime;
    }

    Serial.print("secondsSinceLastUpdate: ");
    Serial.println(secondsSinceLastUpdate);

    Serial.print("currentTime: ");
    Serial.println(currentTime);

    Serial.print("indexTime: ");
    Serial.println(indexTime);

    if (secondsSinceLastUpdate >= secondsTick)
    {
        updateMatrix();
        secondsSinceLastUpdate = 0;
    }

    Serial.print("secondsSinceLastUpdate: ");
    Serial.println(secondsSinceLastUpdate);

    delay(500);
}

uint32_t calculateTick(int duration)
{
    float tick = duration / lightsInMatrix;
    return floor(tick);
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

    m.setDot(currentColumn, currentRow, true);
}
