#include "idle.h"

void setupIdle() 
{

}

void startIdle()
{
    bool state = random(0, 2);
    byte column = random(0, 8);
    int32_t row = random(0, 8);

    setMatrix(column, row, state);
}