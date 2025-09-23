#include "Vex5.h"
#include "string.h"
// #include "Vex5_Optical.h"

extern uint8_t ver;

int8_t Vex5_Optical::begin(VEX5_PORT_t port, Vex5_t* shield)
{
  this->shield = shield;
  int8_t result = -1;
  if((port > VEX5_PORT_NO) && (port <= VEX5_PORT_12))
  {
    uint8_t data[256];
    if(ver == 5) {                        //Process port numbers for version 5
      if(port <= VEX5_PORT_3) {
        id = 1;
        this->port = port;
      } else if (port > VEX5_PORT_9) {
        id = 1;
        this->port = port - 6;
      } else {
        id = 0;
        this->port = port - 3;
      }
    } else {                              //Process port numbers for version 0
  	  if(port > VEX5_PORT_6)
  	  {
  	    id = 1;
  	    this->port  = port - 6;
  	  }
  	  else
  	  {
        id = 0;
        this->port = port;
  	  }
    }
  	data[0] = this->port - 1;
  	while(shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_BEGIN, data, 1))
      delay(50);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
  	}
  }
  return result;
}

int8_t Vex5_Optical::getColor(uint16_t &red_channel, uint16_t &green_channel, uint16_t &blue_channel)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_GET_COLOR, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      red_channel = data[0];
      red_channel |= ((uint16_t)data[1]) << 8;
      green_channel = data[2];
      green_channel |= ((uint16_t)data[3]) << 8;
      blue_channel = data[4];
      blue_channel |= ((uint16_t)data[5]) << 8;
    }
  }
  return result;
}

int8_t Vex5_Optical::getBrightness(uint16_t &brightness)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_GET_BRIGHTNESS, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      brightness = data[0];
      brightness |= ((uint16_t)data[1]) << 8;
    }
  }
  return result;
}

int8_t Vex5_Optical::getRange(int16_t &range)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_GET_RANGE, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      range = data[0];
    }
  }
  return result;
}

int8_t Vex5_Optical::getRawDirection(int16_t &dir)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_GET_DIRECTION, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      dir = data[0];
    }
  }
  return result;
}

int8_t Vex5_Optical::getDirection(int16_t &x, int16_t &y)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_GET_DIRECTION, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      switch (data[0])
      {
        case 1:
          x = 0;
          y = 1;
          break;
        case 2:
          x = 0;
          y = -1;
          break;
        case 3:
          x = 1;
          y = 0;
          break;
        case 4:
          x = -1;
          y = 0;
          break;
        default:
          x = 0;
          y = 0;
      }
    }
  }
  return result;
}

int8_t Vex5_Optical::setLed(int8_t led_state)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    data[1] = led_state;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_SET_LED, data, 2);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
    result = 0;
    }
  }
  return result;
}

int8_t Vex5_Optical::setDirection(int8_t dir_state)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    data[1] = dir_state;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_OPTICAL_SET_DIRECTION, data, 2);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
    result = 0;
    }
  }
  return result;
}
