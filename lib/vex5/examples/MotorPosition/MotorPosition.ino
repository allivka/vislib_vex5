#include <Vex5.h>

Vex5_Motor motor;
int32_t goalPosition = 0;
int16_t maxSpeed = 200;
int32_t realPosition = 0;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  motor.begin(VEX5_PORT_1);
}

void loop() {
  motor.setPosition(goalPosition, maxSpeed);
  delay(2000);
  motor.getPosition(realPosition);
  Serial.println(realPosition);
  goalPosition += 90*2.5;
}