#include "Vex5.h"
#include "string.h"
// #include "Vex5_Distance.h"

extern uint8_t ver;

int8_t Vex5_Distance::begin(VEX5_PORT_t port, Vex5_t* shield)
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

int8_t Vex5_Distance::getValue(int16_t &distance)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_DISTANCE_GET_VALUE, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      distance = data[0];
      distance |= ((int16_t)data[1]) << 8;
    }
  }
  return result;
}

