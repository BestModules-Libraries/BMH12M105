/*****************************************************************
File:         readWeightWithUART.ino
Description:  Read the current module measurement results after initializing the module
connection method：
Note:
******************************************************************/
#include "BMH12M105.h"
//BMH12M105 weight(&Serial1); // Please comment out this line of code if you use Wire on BMduino
//BMH12M105 weight(&Serial); // Please uncomment out this line of code if you use Wire on BMduino
BMH12M105 weight(6,7); // RX TX .Please uncomment out this line of code if you use Wire on BMduino
int16_t readData;
void setup()
{
   Serial.begin(9600); // start serial for output
   weight.begin();
}
void loop()
{
   // Get weight data
   readData=weight.readWeight();
   Serial.print(readData);
   Serial.println("g");
   delay(100);
}
