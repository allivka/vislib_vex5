#include <Vex5.h>

Vex5_Optical optical_sensor;

int range;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  optical_sensor.begin(VEX5_PORT_1);
}

void loop() {
  optical_sensor.getRange(range);

  if (range < 200)
    optical_sensor.setLed(0);
  else
    optical_sensor.setLed(1);

  delay(100);
}
