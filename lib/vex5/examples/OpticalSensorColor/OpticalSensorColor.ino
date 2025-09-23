#include <Vex5.h>

Vex5_Optical optical_sensor;

void setup() {
  Serial.begin(115200);
  Vex5.begin();
  optical_sensor.begin(VEX5_PORT_1);
}

void loop() {
  uint16_t red, green, blue, brightness;
  
  optical_sensor.getColor(red, green, blue);
  optical_sensor.getBrightness(brightness);

  Serial.print("red   = "); Serial.println(red);
  Serial.print("green = "); Serial.println(green);
  Serial.print("blue  = "); Serial.println(blue);
  Serial.print("brightness= "); Serial.println(brightness);
  
  delay(500);
}
