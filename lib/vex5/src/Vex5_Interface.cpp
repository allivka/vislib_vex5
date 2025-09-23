/*
 * Vex5_Interface.cpp
 *
 *  Created on: 10 мая 2019 г.
 *      Author: potor
 */
#include "Vex5_Interface.h"
#include "SPI.h"

Vex5_Interface::Vex5_Interface()
{
  isBegin = false;
  cs_pin = -1;
}

Vex5_Interface::begin(int32_t speed, int8_t chip_select)
{
  if(isBegin == false)
  {
    isBegin = true;
    SPI.begin();
    cs_pin = chip_select;
    if(cs_pin >= 0)
    {
      pinMode(chip_select, OUTPUT);
      digitalWrite(cs_pin, HIGH);
    }
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
  }
}

void Vex5_Interface::write(uint8_t *buf, uint8_t len)
{
  if(cs_pin >= 0)
  {
    digitalWrite(cs_pin, LOW);
  }
  for(int i = 0; i < len; i++)
  {
    SPI.transfer(buf[i]);
  }
  if(cs_pin >= 0)
  {
    digitalWrite(cs_pin, HIGH);
  }
}

void Vex5_Interface::start_reading()
{
  if(cs_pin >= 0)
  {
    digitalWrite(cs_pin, LOW);
  }
}

void Vex5_Interface::stop_reading()
{
  if(cs_pin >= 0)
  {
    digitalWrite(cs_pin, HIGH);
  }
}

uint8_t Vex5_Interface::read()
{
  uint8_t result;
  result = SPI.transfer(0);
  return result;
}

