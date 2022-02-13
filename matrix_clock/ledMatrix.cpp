#include "ledMatrix.h"

//LED Matix
const byte DIN = 7; // DIN pin of MAX7219 module
const byte CLK = 6; // CLK pin of MAX7219 module
const byte CS = 5;  // CS pin of MAX7219 module
const byte maxInUse = 1;

MaxMatrix matrix(DIN, CS, CLK, maxInUse);

void setupMatrix()
{
    //LED Matix init
    matrix.init();
    matrix.setIntensity(0); // initial led matrix intensity, 0-15
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

    Serial.print("CLEAR: ");
    Serial.print(currentRow);
    Serial.print(" x ");
    Serial.println(currentColumn);

    matrix.setDot(currentColumn, currentRow, false);
}

void alarm()
{
    Serial.println("Alarm");

    while (true)
    {
        fillMatrix();
        delay(250);

        clearMatrix();
        delay(250);
    }
}

void flashCursor(uint32_t currentColumn, int32_t currentRow)
{
    Serial.print("FLASH: ");
    Serial.print(currentRow);
    Serial.print(" x ");
    Serial.println(currentColumn);

    matrix.setDot(currentColumn, currentRow, true);
    delay(250);
    matrix.setDot(currentColumn, currentRow, false);
    delay(250);
}

void clearMatrix()
{
    matrix.clear();
}

void fillMatrix()
{
    for (int i = 0; i < 8; i++)
    {
        matrix.setColumn(i, B11111111);
    }
}