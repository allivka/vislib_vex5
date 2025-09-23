#include "Vex5.h"
#include "string.h"
extern uint8_t ver;

int8_t Vex5_Motor::begin(VEX5_PORT_t port, Vex5_t* shield)
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

int8_t Vex5_Motor::setPower(int8_t power)
{
  int8_t result = -1;
  if (port)
  {
   int16_t speed = power * 23;
   uint8_t data[256];
   data[0] = port - 1;
   data[1] = speed;
   data[2] = speed >> 8;
   shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_SET_SPEED, data, 3);
   int8_t len = shield->receiveData(data);
   if(len > 0)
   {
	  result = 0;
   }
  }
  return result;
}

int8_t Vex5_Motor::setPosition(int32_t angle, int16_t max_speed)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    data[1] = max_speed;
    data[2] = max_speed >> 8;
    data[3] = angle;
    data[4] = angle >> 8;
    data[5] = angle >> 16;
    data[6] = angle >> 24;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_SET_POSITION, data, 7);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
 	  result = 0;
    }
  }
  return result;
}

int8_t Vex5_Motor::setSpeed(int16_t speed)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    data[1] = speed;
    data[2] = speed >> 8;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_SET_SPEED, data, 3);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
	     result = 0;
    }
  }
  return result;
}

int8_t Vex5_Motor::setMaxCurrent(int16_t maxCurrent)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    data[1] = maxCurrent;
    data[2] = maxCurrent >> 8;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_SET_MAX_CURRENT, data, 3);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
	  result = 0;
    }
  }
  return result;
}

int8_t Vex5_Motor::resetEncoder()
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_ENCODER_RESET, data, 1);
	  int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
    }
  }
  return result;
}

int8_t Vex5_Motor::getPower(int16_t &power)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_GET_POWER, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      power = data[0];
      power |= ((int16_t)data[1]) << 8;
    }
  }
  return result;
}

int8_t Vex5_Motor::getSpeed(int16_t &speed)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_GET_SPEED, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      speed = data[0];
      speed |= ((int16_t)data[1]) << 8;
    }
  }
  return result;
}

int8_t Vex5_Motor::getPosition(int32_t &position)
{
  int8_t result = -1;
  if (port)
  {
    uint8_t data[256];
    data[0] = port - 1;
    shield->sendData((VEX5_DEVICE_ID_t)id, VEX5_CMD_MOTOR_GET_POSITION, data, 1);
    int8_t len = shield->receiveData(data);
    if(len > 0)
    {
      result = 0;
      position = data[0];
      position |= ((int32_t)data[1]) << 8;
      position |= ((int32_t)data[2]) << 16;
      position |= ((int32_t)data[3]) << 24;
    }
  }
  return result;
}

int16_t Vex5_Motor::getPower()
{
  int16_t result;
  while(getPower(result))
    ;
  return result;
}
int16_t Vex5_Motor::getSpeed()
{
  int16_t result;
  while(getSpeed(result))
    ;
  return result;
}
int32_t Vex5_Motor::getPosition()
{
  int32_t result;
  while(getPosition(result))
    ;
  return result;
}
