/*****************************************************************
File:         readWeightWithIIC.ino
Description:  Read the current module measurement results after initializing the module
connection method：
Note:
******************************************************************/
#include "BMH12M105.h"
BMH12M105 weight(&Wire,0x50); // Please comment out this line of code if you use Wire on BMduino
int16_t readData;
void setup()
{
   Serial.begin(9600); // start serial for output
   weight.begin();
   weight.calibrationZero();//Calibration zero point,No need to run, can be annotated
}
void loop()
{
   // Get weight data
   readData=weight.readWeight();
   Serial.print(readData);
   Serial.println("g");
   delay(100);
}
