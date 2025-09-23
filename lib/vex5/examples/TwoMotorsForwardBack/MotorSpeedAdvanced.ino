#include <Vex5.h>

#define BAUDRATE         (500000) //SPI speed
#define SPI_NSS_PIN      (-1)     //Use jumper

Vex5_Motor motor[2];
int16_t goalSpeed[2] = {200, 300};
int16_t realSpeed[2] = {0, 0};
int32_t realPosition[2] = {0, 0};
int16_t realPower[2] = {0, 0};
unsigned long time = millis();

void setup() {
  Serial.begin(115200);
  Vex5.begin(BAUDRATE, SPI_NSS_PIN);
  motor[0].begin(VEX5_PORT_1);
  motor[1].begin(VEX5_PORT_7);
  time = millis();
}

void loop() {
  for(int i = 0; i < 2; i++)
  {
    motor[i].setSpeed(goalSpeed[i]);
    motor[i].getSpeed(realSpeed[i]);
    motor[i].getPosition(realPosition[i]);
    motor[i].getPower(realPower[i]);
  }

  Serial.println("Pos[0]: " + String(realPosition[0]) + "\tSpeed[0]: " + String(realSpeed[0]) + "\tPower[0]: " + String(realPower[0]) + 
               "\tPos[1]: " + String(realPosition[1]) + "\tSpeed[1]: " + String(realSpeed[1]) + "\tPower[1]: " + String(realPower[1]));

  if(millis() - time > 1000) {
    time = millis();
    goalSpeed[0] = -goalSpeed[0];
    goalSpeed[1] = -goalSpeed[1];
  }
  delay(300);
}