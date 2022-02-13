#include <MaxMatrix.h>

//devide recived time by 64
//start countdown
//flash matrix and sound alarm when time has elapsed

int DIN = 7;   // DIN pin of MAX7219 module
int CLK = 6;   // CLK pin of MAX7219 module
int CS = 5;    // CS pin of MAX7219 module
int maxInUse = 1;

MaxMatrix m(DIN, CS, CLK, maxInUse); 

void setup() {
  m.init(); // MAX7219 initialization  
  m.setIntensity(0); // initial led matrix intensity, 0-15

}

void loop() {
  for (int r=8; r>-1; r--){
    for (int c=8; c>-1; c--){
      m.setDot(c,r,true); 
      delay(100);
    
      m.setDot(c,r,false); 
    }
  }
}
