/*
 * Vex5_InternalProtocol.h
 *
 *  Created on: 10 мая 2019 г.
 *      Author: potor
 */

#ifndef VEX5_INTERNAL_PROTOCOL_H_
#define VEX5_INTERNAL_PROTOCOL_H_

#include "Vex5_Interface.h"

typedef enum
{
  VEX5_DEVICE_ID_1,
  VEX5_DEVICE_ID_2
}VEX5_DEVICE_ID_t;

typedef enum
{
  VEX5_REAL_PORT_NONE,
  VEX5_REAL_PORT_1,
  VEX5_REAL_PORT_2,
  VEX5_REAL_PORT_3,
  VEX5_REAL_PORT_4,
  VEX5_REAL_PORT_5,
  VEX5_REAL_PORT_6
}VEX5_REAL_PORT_t;


typedef enum
{
  VEX5_CMD_START = 0,
  VEX5_CMD_UPDATE_FIRMWARE,
  VEX5_CMD_NACK,
  VEX5_CMD_ERASE,
  VEX5_CMD_MOTOR_BEGIN,
  VEX5_CMD_MOTOR_SET_SPEED,
  VEX5_CMD_MOTOR_SET_POSITION,
  VEX5_CMD_MOTOR_SET_MAX_CURRENT,
  VEX5_CMD_MOTOR_ENCODER_RESET,
  VEX5_CMD_MOTOR_GET_POWER,
  VEX5_CMD_MOTOR_GET_SPEED,
  VEX5_CMD_MOTOR_GET_POSITION,
  VEX5_CMD_POWER_ON,
  VEX5_CMD_RESET,
  VEX5_CMD_GET_HW_VERSION,
  VEX5_CMD_POWER_ON_X,
  VEX5_CMD_DISTANCE_GET_VALUE,
  VEX5_CMD_OPTICAL_GET_COLOR,
  VEX5_CMD_OPTICAL_GET_BRIGHTNESS,
  VEX5_CMD_OPTICAL_GET_RANGE,
  VEX5_CMD_OPTICAL_GET_DIRECTION,
  VEX5_CMD_OPTICAL_SET_LED,
  VEX5_CMD_OPTICAL_SET_DIRECTION
}VEX5_CMD_t;

class Vex5_InternalProtocol
{
private:
  static const uint16_t Crc16Table[256];
  static const uint8_t linkMarker;
  Vex5_Interface interface;
  uint16_t crc16Calculate(uint8_t *data, uint16_t lenght);
public:
  Vex5_InternalProtocol(){};
  void begin_internal_protocol(int32_t speed = 500000, int8_t chip_select = -1);
//  void begin() {interface.begin(baudrate, chip_select);}
  int8_t sendData(VEX5_DEVICE_ID_t devId, VEX5_CMD_t cmd, uint8_t *data, uint8_t len);
  int8_t receiveData(uint8_t *data);
  void resync();
};

#endif /* SRC_INTERNAL_PROTOCOL_H_ */
