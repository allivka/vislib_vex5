#ifndef VEX5_DISTANCE_H_
#define VEX5_DISTANCE_H_
#include <Arduino.h>
#include "Vex5.h"

class Vex5_Distance : public Vex5_InternalProtocol
{
private:
  Vex5_t* shield;
  int port;
  int id;
public:
  Vex5_Distance() : port(0){};
  int8_t begin(VEX5_PORT_t port, Vex5_t *shield = &Vex5);
  int8_t getValue(int16_t &distance);
};

#endif