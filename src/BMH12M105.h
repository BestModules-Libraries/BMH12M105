
/*************************************************
File:       		BMH12M105.h
Author:             BESTMODULE
Description:        Define classes and required variables
History：
V1.0.2	 -- initial version；2024-01-11

**************************************************/
#ifndef _BMH12M105_H__
#define _BMH12M105_H__

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define BMH12M105_IICADDR 0x50
#define CHECK_OK        0
#define CHECK_ERROR     1
#define TIMEOUT_ERROR   2


class BMH12M105
{
public:
     BMH12M105(TwoWire *theWire= &Wire,uint8_t i2c_addr= BMH12M105_IICADDR);
     BMH12M105(HardwareSerial*theSerial=&Serial);
     BMH12M105(uint8_t rxPin, uint8_t txPin);
     void begin();
     uint16_t readWeight();
     uint16_t readRawADC();
     uint8_t readStatus();
     uint8_t calibrationZero();
     uint8_t calibrationMax(uint16_t maxValue);
     void sleep();
     void wake();
     

private:
     uint8_t _i2cAddress;
     TwoWire *_wire = NULL;
     uint8_t _rxPin;
     uint8_t _txPin;
     HardwareSerial*_serial =NULL;
     SoftwareSerial *_softSerial =NULL;
     //iic
     void writeBytes(uint8_t wbuf[], uint8_t wlen);
     uint8_t readBytes(uint8_t rbuf[], uint8_t rlen,uint8_t timeout);
     //uart
     uint8_t getChecksum(uint8_t Buf[]);
     uint8_t writeRegUart(uint8_t regAdd, uint16_t regData);
     uint16_t readRegUart(uint8_t regAdd);
};

#endif
