#include "Vex5.h"
#include "firmware.h"
#include "firmware_hw5.h"
#include "string.h"

Vex5_t Vex5;
uint8_t ver;
int8_t Vex5_t::begin(int32_t speed, int8_t chip_select, int8_t is_reset)
{
  begin_internal_protocol(speed, chip_select);
  int8_t result = -1;
  uint8_t data[256];
  delay(100);
  
  resync();
  delay(100);

  while(getHwVersion(ver))
    ;
  Serial.begin(115200);
  Serial.println(ver);
  if(ver == 0)
  {
    if(is_reset)
    {
      sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_RESET, data, 0);
      sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_RESET, data, 0);
      sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_RESET, data, 0);
      sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_RESET, data, 0);
      delay(500);
    }


    sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_POWER_ON, data, 0);
    if(receiveData(data) == -1)
      return -1;
    sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_START, data, 0);
    delay(300);
  }
  else if(ver == 5)
  {
    sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_START, data, 0);
    delay(100);

    Serial.println("Tell MCU0 to Power ON");
    data[0] = (VEX5_DEVICE_ID_t)0;
    sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_POWER_ON_X, data, 1);
    delay(15);
    if(receiveData(data) == -1)
      return -1;

    delay(2);
    Serial.println("Tell MCU1 that MCU0 is powered on");
    data[0] = (VEX5_DEVICE_ID_t)0;
    sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_POWER_ON_X, data, 1);
    delay(15);
    if(receiveData(data) == -1)
      return -1;

    delay(150);

    Serial.println("Tell MCU1 to power ON");
    data[0] = (VEX5_DEVICE_ID_t)1;
    sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_POWER_ON_X, data, 1);
    delay(15);
    if(receiveData(data) == -1)
      return -1;

    delay(2);
    Serial.println("Tell MCU0 that MCU1 is powered on");
    data[0] = (VEX5_DEVICE_ID_t)1;
    sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_POWER_ON_X, data, 1);
    delay(30);
    if(receiveData(data) == -1)
      return -1;

    delay(300);
  }

  return 0;
}

int8_t Vex5_t::begin_bootloader(int32_t speed, int8_t chip_select)
{
  begin_internal_protocol(speed, chip_select);
  delay(100);
  resync();
  delay(100);
  return 0;
}

int8_t Vex5_t::updateFirmware()
{
  uint8_t ver = 0;
  getHwVersion(ver);
  Serial.println("HW version: " + String(ver));

  if(ver == 5)
    return updateFirmwareHw5();
  else
    return updateFirmwareHw0();

  Serial.println("Error: unknown HW version");
  return -1;
}

int8_t Vex5_t::updateFirmwareHw0()
{
  uint8_t data[256];

  for(int deviceNumber = 0; deviceNumber < 2; deviceNumber++)
  {
    int8_t result = -1;
    uint8_t size = sizeof(firmwareData) / 2048;
    if(sizeof(firmwareData) % 2048)
    {
  	 size++;
    }
    data[0] = size;
    sendData((VEX5_DEVICE_ID_t)deviceNumber, VEX5_CMD_ERASE, data, 1);
    delay(200);
    if(receiveData(data) == -1)
    {
      return -1 - deviceNumber * 10;
    }

    for (uint32_t i = 0; i < sizeof(firmwareData); i += 64)
    {
      data[0] = i;
    	data[1] = i >> 8;
    	data[2] = i >> 16;
    	data[3] = i >> 24;
    	for(int j = 0; j < 64; j++)
    	{
          data[4 + j] = pgm_read_byte_near(firmwareData + i + j);
    	}
    	sendData((VEX5_DEVICE_ID_t)deviceNumber, VEX5_CMD_UPDATE_FIRMWARE, data, 64 + 4);
    	delay(30);
      if(receiveData(data) == -1)
      {
        return -2 - deviceNumber * 10;
      }
    }
  }

  sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_START, data, 0);
  delay(10);
  sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_START, data, 0);
  delay(10);

  return 0;
}

int8_t Vex5_t::updateFirmwareHw5()
{
  uint8_t data[256];

  for(int deviceNumber = 0; deviceNumber < 2; deviceNumber++)
  {
    Serial.println("MCU " + String(deviceNumber + 1));
    int8_t result = -1;
    uint8_t size = sizeof(firmwareDataHw5) / 128;
    if(sizeof(firmwareDataHw5) % 128)
    {
     size++;
    }
    data[0] = size;
    sendData((VEX5_DEVICE_ID_t)deviceNumber, VEX5_CMD_ERASE, data, 1);
    delay(1000);
    Serial.println("Erasing ...");
    if(receiveData(data) == -1)
    {
      Serial.println("Can't erase");
      return -1 - deviceNumber * 10;
    }

    for (uint32_t i = 0; i < sizeof(firmwareDataHw5); i += 64)
    {
      Serial.println("Writing block  " + String(i + 1) + "/" + String(sizeof(firmwareDataHw5)));
      data[0] = i;
      data[1] = i >> 8;
      data[2] = i >> 16;
      data[3] = i >> 24;
      for(int j = 0; j < 64; j++)
      {
          data[4 + j] = pgm_read_byte_near(firmwareDataHw5 + i + j);
      }
      sendData((VEX5_DEVICE_ID_t)deviceNumber, VEX5_CMD_UPDATE_FIRMWARE, data, 64 + 4);
      delay(150);
      if(receiveData(data) == -1)
      {
        return -2 - deviceNumber * 10;
      }
    }
  }

  sendData((VEX5_DEVICE_ID_t)1, VEX5_CMD_START, data, 0);
  delay(10);
  sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_START, data, 0);
  delay(10);

  return 0;
}

int8_t Vex5_t::getHwVersion(uint8_t &hwVersion)
{
  int8_t result = -1;

  uint8_t data[256];
  data[0] = 0;
  sendData((VEX5_DEVICE_ID_t)0, VEX5_CMD_GET_HW_VERSION, data, 1);
  delay(10);
  int8_t len = receiveData(data);
  if(len > 0)
  {
    result = 0;
    hwVersion = data[0];
  }

  return result;
}
