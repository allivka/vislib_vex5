/*
 * interface.h
 *
 *  Created on: 10 ��� 2019 �.
 *      Author: potor
 */

#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include <Arduino.h>

class Vex5_Interface
{
  bool isBegin;
  int8_t cs_pin;
public:
  Vex5_Interface();
  begin(int32_t speed, int8_t chip_select);
  void write(uint8_t *buf, uint8_t len);
  uint8_t read();
  void start_reading();
  void stop_reading();
};

#endif /* SRC_INTERFACE_H_ */
