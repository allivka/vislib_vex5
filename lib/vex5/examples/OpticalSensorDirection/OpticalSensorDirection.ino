#include <Vex5.h>

Vex5_Optical optical_sensor;

uint16_t red, green, blue, brightness;
int x, y, range;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  optical_sensor.begin(VEX5_PORT_1);
}

void loop() {
  optical_sensor.setDirection(1);

  optical_sensor.getDirection(x, y);

  Serial.print("x = "); Serial.println(x); 
  Serial.print("y = "); Serial.println(y); 

  delay(100);
}
