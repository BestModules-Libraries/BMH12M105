/*************************************************
File:       			BMH12M105.cpp
Author:           BESTMODULE
Description:      I2C communication with the BMH12M105-1 and obtain the corresponding value 
History：			  
V1.0.2	 -- initial version；2024-01-11

**************************************************/
#include "BMH12M105.h"

/*************************************************
Description:  Constructor
Input:        *theWire: Wire object if your board has more than one I2C interface
              addr: IIC address
Output:
Others:
*************************************************/
BMH12M105::BMH12M105(TwoWire *theWire,uint8_t i2c_addr)
{
   _wire = theWire;  
   _i2cAddress = i2c_addr;
   _softSerial = NULL;
   _serial = NULL;
}
/**********************************************************
  Description: Select the hardware serial port you need to use
  Parameters:       *theSerial：hardware serial
             BMduino optional:serial(default) serial1/seria2/seria3/seria4
             UNO optional:serial(default)
  Return:      none
  Others:
**********************************************************/
BMH12M105::BMH12M105(HardwareSerial*theSerial)
{
  _serial = theSerial;
  _softSerial = NULL;
}
/**********************************************************
  Description: Select the software serial port RX TX you need to use
  Parameters:       rxPin:RX pin on the development board
             txPin:TX pin on the development board
      none
  Return:      none
  Others:
**********************************************************/
BMH12M105::BMH12M105(uint8_t rxPin, uint8_t txPin)
{
  _serial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  _softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/*************************************************
Description:  Module Initial
Parameters: 
Return:
Others:
*************************************************/
void BMH12M105::begin()
{
  if (_wire != NULL)
  {
    _wire->begin();
  }
  if (_softSerial != NULL)
  {
    _softSerial->begin(9600);
  }
  if (_serial != NULL)
  {
    _serial->begin(9600);
  }
}
/**********************************************************
Description: ReadWeight
Parameters:  wValue 
Return:      wValue :2 byte
Others:      none
**********************************************************/
uint16_t BMH12M105::readWeight()
{
  uint8_t rbuf[2]={0};  
  uint16_t wValue=0;
  if (_wire != NULL)
  {
  _wire->beginTransmission(_i2cAddress);
  _wire->write(0x89);
  _wire->endTransmission();
  if(readBytes(rbuf,2,50)==0x00)
  {
  wValue=rbuf[1] << 8 | rbuf[0];
  }
  return wValue;
  }
  else
  {
    wValue=readRegUart(0x89);
    return wValue;
  }
  
}
/**********************************************************
Description: Readad
Parameters:  adValue    
Return:      adValue :2 byte
Others:      none
**********************************************************/
uint16_t BMH12M105::readRawADC()
{
  uint8_t rbuf[2]={0};
  uint16_t adValue=0;
  if (_wire != NULL)
  {
  _wire->beginTransmission(_i2cAddress);
  _wire->write(0x80);
  _wire->endTransmission();
  if (readBytes(rbuf,2,50)==0x00)
  {
  adValue=rbuf[1] << 8 | rbuf[0];
  }
  return adValue;
  }
  else
  {
    adValue=readRegUart(0x80);
    return adValue;
  }
}
/**********************************************************
Description: Read calibration status
Parameters:  none     
Return:      status :1byte
             0x01: Zero calibration in progress/tarring in progress
             0x02: Calibrating the highest point
             0x03: Calibration succeeded
             0x04: Calibration failed
Others:      none
**********************************************************/
uint8_t BMH12M105::readStatus()
{
  uint8_t rbuf[2]={0};
  uint8_t Status=0;
  if (_wire != NULL)
  {
  _wire->beginTransmission(_i2cAddress);
  _wire->write(0x88);
  _wire->endTransmission();
  if (readBytes(rbuf,1,50)==0x00)
  {
  return rbuf[0];
  }
  return 0;
  }
  else
  {
    Status=readRegUart(0x88);
    return Status;
  }
}
/**********************************************************
Description: Zero calibration/tare
Parameters:  none     
Return:      status :1byte
             0 succeeded 1 failed
Others:      none
**********************************************************/
uint8_t BMH12M105::calibrationZero()
{
  uint8_t wbuf[3]={0x91,0x01,0x00};
  uint8_t Status=0;
  if (_wire != NULL)
  {
  writeBytes(wbuf,3);
  Status=readStatus();
  }
  else
  {
   if(writeRegUart(0x91,0x0001)==true)
   {
   Status=0;
   }
  }
  return Status;
}
/**********************************************************
Description: Calibrate the highest point
Parameters:  none     
Return:      status :1byte
             0x01: Zero calibration in progress/tarring in progress
             0x02: Calibrating the highest point
             0x03: Calibration succeeded
             0x04: Calibration failed
Others:      none
**********************************************************/
uint8_t BMH12M105::calibrationMax(uint16_t maxValue)
{
  uint8_t wbuf[3]={0x8F,0X00,0x00};
    if (_wire != NULL)
  {
  wbuf[1]=lowByte(maxValue);
  wbuf[2]=highByte(maxValue);
  writeBytes(wbuf,3);
  return readStatus();
  }
  else
  {
   writeRegUart(0x8F,maxValue);
   return readStatus();
  }
  
}
/**********************************************************
Description: Sleep module
Parameters:  none     
Return:      none 
Others:      none
**********************************************************/
void BMH12M105::sleep()
{
  uint8_t wbuf[3]={0x92,0X01};
  if (_wire != NULL)
  {
  writeBytes(wbuf,2);
  }
  else
  {
    writeRegUart(0x92,0x01);
  }
}
/**********************************************************
Description: Wake up module
Parameters:  none     
Return:      none 
Others:      none
**********************************************************/
void BMH12M105::wake()
{
    if (_wire != NULL)
  {
  uint8_t wbuf[3]={0x92,0x00};
  writeBytes(wbuf,2); 
  }
  else
  {
    writeRegUart(0x92,0x00);
  }
}

/**********************************************************
Description: Write data through IIC
Parameters: wbuf:The array for storing Data to be sent
            wlen:Length of data sent
Return: None
Others: None
**********************************************************/
void BMH12M105::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  if (_wire != NULL)
  {
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(_i2cAddress);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
  }
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  if (_serial != NULL)
  {
    while (_serial->available() > 0)
    {
      _serial->read();
    }
    _serial->write(wbuf, wlen);
  }
}
/**********************************************************
Description: Read data through IIC
Parameters: rbuf: Used to store received data
            rlen: Length of data to be read
Return:  0:OK
         2: timeout error
Others: None
**********************************************************/
uint8_t BMH12M105::readBytes(uint8_t rbuf[], uint8_t rlen,uint8_t timeout)
{
  uint8_t i = 0, delayCnt = 0,checkSum=0;
  if (_wire != NULL)
  {
  _wire->requestFrom(_i2cAddress, rlen);
  while (_wire->available())
  {
    for (i = 0; i < rlen; i++)
    {
      
      rbuf[i] = _wire->read();
    }
    return 0;
  }
  return 1;
  }
  else
  {
/* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_serial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _serial->read();
    }
  }
  /* check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  checkSum = ~checkSum + 1;
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
  }
}


/**********************************************************
Description: Write data through IIC
Parameters:  Buf[]:txBuf:6-byte array to be sent
Return: checksum:Equal to the sum of data of other bytes
Others: None
**********************************************************/
uint8_t BMH12M105::getChecksum(uint8_t Buf[])
{
  uint8_t checksum = 0;
  checksum = 0;
  for (uint8_t i = 0; i < 5; i++)
  {
    checksum += Buf[i];
  }
  checksum = ~checksum + 1;
  return checksum;
}
/**********************************************************
Description: Write data to the register of the destination address
Parameters:  regAdd:Register of destination address
             regData:Data to be written
Return: 
         true/false
Others: None
**********************************************************/
uint8_t BMH12M105::writeRegUart(uint8_t regAdd, uint16_t regData)
{
  uint8_t readFlag = 1;
  uint8_t txBuf[6]={0x55,0x01,regAdd,0x00,0x00,0x00};
  uint8_t rxBuf[6];
  txBuf[3] = lowByte(regData);
  txBuf[4] = highByte(regData);
  txBuf[5] = getChecksum(&txBuf[0]);
  writeBytes(txBuf, 6);
  delay(50);
  readFlag = readBytes(rxBuf, 6,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
      return true;
    }
    else
    {
      return false;
    }
}
/**********************************************************
Description: Read the data in the register of the target address
Parameters:  regAdd:Register of destination address
             
Return: value:Register data
Others: None
**********************************************************/
uint16_t BMH12M105::readRegUart(uint8_t regAdd)
{
  uint8_t readFlag = 1;
  uint16_t value=0;
  uint8_t txBuf[6]={0x55,0x00,regAdd,0x00,0x00,0x00};
  uint8_t rxBuf[6];
  txBuf[5] = getChecksum(&txBuf[0]);
  writeBytes(txBuf, 6);
    delay(50);
    readFlag=readBytes(rxBuf, 6,100);
    if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
      if (rxBuf[2] == 0x88 || rxBuf[2] == 0x92 || rxBuf[2] == 0x93)
    {  
      Serial.println(rxBuf[3],DEC);  
      value = rxBuf[3];
    }
    else
    {
      value = rxBuf[4] << 8 | rxBuf[3];
    }
    } 
   return value;
}
