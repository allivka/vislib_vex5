#include <Vex5.h>

Vex5_Distance distance_sensor;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  distance_sensor.begin(VEX5_PORT_1);
}

void loop() {
  delay(500);
  int length = 0;
  distance_sensor.getValue(length);
  Serial.print("length = ");
  Serial.print(length);
  Serial.println(" mm");
}
