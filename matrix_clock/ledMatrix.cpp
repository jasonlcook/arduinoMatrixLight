#include "ledMatrix.h"

void updateMatrix(MaxMatrix matrix, uint32_t &currentColumn, int32_t &currentRow)
{
    currentRow -= 1;
    if (currentRow < 0)
    {
        currentRow = 7;
        currentColumn += 1;
        if (currentColumn > 7)
        {
            alarm(matrix);
        }
    }

    Serial.print("CLEAR: ");
    Serial.print(currentRow);
    Serial.print(" x ");
    Serial.println(currentColumn);

    //matrix.setDot(currentColumn, currentRow, false);
}

void alarm(MaxMatrix matrix)
{
    Serial.println("Alarm");

    while (true)
    {
        fillMatrix(matrix);
        delay(250);

        clearMatrix(matrix);
        delay(250);
    }
}

void flashCursor(MaxMatrix matrix, uint32_t currentColumn, int32_t currentRow)
{
    Serial.print("FLASH: ");
    Serial.print(currentRow);
    Serial.print(" x ");
    Serial.println(currentColumn);

    matrix.setDot(currentColumn, currentRow, true);
    delay(100);
    matrix.setDot(currentColumn, currentRow, false);
    delay(100);
    matrix.setDot(currentColumn, currentRow, true);
    delay(100);
    matrix.setDot(currentColumn, currentRow, false);
    delay(100);
}

void clearMatrix(MaxMatrix matrix)
{
    matrix.clear();
}

void fillMatrix(MaxMatrix matrix)
{
    for (int i = 0; i < 8; i++)
    {
        matrix.setColumn(i, B11111111);
    }
}