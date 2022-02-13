#include <Wire.h>
#include <DS3231.h>

#include <MaxMatrix.h>

//todo:
//  -   allow the input and updating of the duration
//  -   invert led matrix to count down instead of up
//  -   set the current digit to blink every seccond
//  -   use a defined tick for each light increment
//  -   breakout functions

  //Time
RTClib myRTC;
uint32_t indexTime;
uint32_t currentTime;
uint32_t elapsedTime;

  //LED Matix
int DIN = 7;   // DIN pin of MAX7219 module
int CLK = 6;   // CLK pin of MAX7219 module
int CS = 5;    // CS pin of MAX7219 module
int maxInUse = 1;

MaxMatrix m(DIN, CS, CLK, maxInUse); 

int currentColumn = 0;
int currentRow = 0;

void setup() {
  //Time init
  Serial.begin(9600);
  Wire.begin();

  //LED Matix init
  m.init(); // MAX7219 initialization  
  m.setIntensity(0); // initial led matrix intensity, 0-15

  //set index time
  DateTime now = myRTC.now();
  indexTime = now.unixtime();

  //set LED matrix
  m.setDot(0, 0, true);
}

void loop() {  
  DateTime now = myRTC.now();
  currentTime = now.unixtime();

  if(currentTime != indexTime)
  {
    //recalculate matrix time
   currentRow += 1;
    if(currentRow > 7)
    {
      currentRow = 0;
      currentColumn += 1;        
      if(currentColumn > 7)
      {
        m.clear();  
        currentRow = 0;
        currentColumn = 0;
      }
    }
    m.setDot(currentColumn, currentRow, true);
    
    currentTime = indexTime;
  }
  
  delay(500);
}
