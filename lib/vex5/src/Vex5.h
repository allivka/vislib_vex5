#ifndef VEX5_H_
#define VEX5_H_
#include <Arduino.h>
#include "Vex5_InternalProtocol.h"

typedef enum
{
  VEX5_PORT_NO,
  VEX5_PORT_1,
  VEX5_PORT_2,
  VEX5_PORT_3,
  VEX5_PORT_4,
  VEX5_PORT_5,
  VEX5_PORT_6,
  VEX5_PORT_7,
  VEX5_PORT_8,
  VEX5_PORT_9,
  VEX5_PORT_10,
  VEX5_PORT_11,
  VEX5_PORT_12,
}VEX5_PORT_t;

class Vex5_t : public Vex5_InternalProtocol
{
  int8_t updateFirmwareHw0();
  int8_t updateFirmwareHw5();
public:
  Vex5();
  int8_t begin(int32_t speed = 500000, int8_t chip_select = -1, int8_t is_reset = 1);
  int8_t begin_bootloader(int32_t speed = 500000, int8_t chip_select = -1);
  int8_t updateFirmware();
  int8_t getHwVersion(uint8_t &hwVersion);
  float deg_to_ticks(float deg) {return deg*2.5;}
  float ticks_to_deg(float ticks) {return ticks/2.5;}
};

extern Vex5_t Vex5;

#include "Vex5_Motor.h"
#include "Vex5_Distance.h"
#include "Vex5_Optical.h"
#endif
