#include <Vex5.h>

Vex5_Distance distance_sensor;
Vex5_Optical optical_sensor;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  distance_sensor.begin(VEX5_PORT_1);
  optical_sensor.begin(VEX5_PORT_7);
}

void loop() {
  int16_t length = 0;
  uint16_t red, green, blue, brightness;
  delay(200);
  distance_sensor.getValue(length);
  delay(100);
  optical_sensor.getColor(red, green, blue);
  delay(100);
  optical_sensor.getBrightness(brightness);
  delay(100);
  Serial.print("length = ");
  Serial.print(length);
  Serial.println(" mm");
  Serial.print("red   = "); Serial.println(red);
  Serial.print("green = "); Serial.println(green);
  Serial.print("blue  = "); Serial.println(blue);
  Serial.print("brightness= "); Serial.println(brightness);
}
