#ifndef VEX5_OPTICAL_H_
#define VEX5_OPTICAL_H_
#include <Arduino.h>
#include "Vex5.h"

class Vex5_Optical : public Vex5_InternalProtocol
{
private:
  Vex5_t* shield;
  int port;
  int id;
public:
  Vex5_Optical() : port(0){};
  int8_t begin(VEX5_PORT_t port, Vex5_t *shield = &Vex5);
  int8_t getColor(uint16_t &red_channel, uint16_t &green_channel, uint16_t &blue_channel);
  int8_t getBrightness(uint16_t &brightness);
  int8_t getRange(int16_t &range);
  int8_t getDirection(int16_t &x, int16_t &y);
  int8_t getRawDirection(int16_t &dir);
  int8_t setLed(int8_t led_state);
  int8_t setDirection(int8_t dir_state);
};

#endif