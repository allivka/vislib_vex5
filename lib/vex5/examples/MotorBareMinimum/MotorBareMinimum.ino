#include <Vex5.h>

Vex5_Motor motor;

void setup() {
  Vex5.begin();
  motor.begin(VEX5_PORT_1);
}

void loop() {
	motor.setSpeed(200);
}