#ifndef VEX5_MOTOR_H_
#define VEX5_MOTOR_H_
#include <Arduino.h>
#include "Vex5.h"

class Vex5_Motor : public Vex5_InternalProtocol
{
private:
  Vex5_t* shield;
  int port;
  int id;
public:
  Vex5_Motor() : port(0){};
  int8_t begin(VEX5_PORT_t port, Vex5_t *shield = &Vex5);
  int8_t setPower(int8_t power);
  int8_t setPosition(int32_t angle, int16_t max_speed);
  int8_t setSpeed(int16_t speed);
  int8_t setMaxCurrent(int16_t maxCurrent);
  int8_t resetEncoder();
  int8_t getPower(int16_t &power);
  int8_t getSpeed(int16_t &speed);
  int8_t getPosition(int32_t &position);
  int16_t getPower();
  int16_t getSpeed();
  int32_t getPosition();
};

#endif
