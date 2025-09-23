#include <Vex5.h>

Vex5_Motor motor;
int16_t goalSpeed = 200;
int16_t realSpeed = 0;
int32_t realPosition = 0;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  motor.begin(VEX5_PORT_1);
}

void loop() {
  motor.setSpeed(goalSpeed);
  delay(2000);
  motor.getSpeed(realSpeed);
  motor.getPosition(realPosition);
  Serial.println(realSpeed);
  Serial.println(realPosition);
  Serial.println();
  goalSpeed = -goalSpeed;
}