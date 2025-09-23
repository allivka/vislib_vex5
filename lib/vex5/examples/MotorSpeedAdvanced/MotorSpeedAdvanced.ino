#include <Vex5.h>

#define BAUDRATE         (500000) //SPI speed
#define SPI_NSS_PIN      (-1)     //Use jumper
#define IS_RESET_SHIELD  (1)      //Reset shield on begin

Vex5_Motor motor;
int16_t goalSpeed = 200;
int16_t realSpeed = 0;
int32_t realPosition = 0;
int16_t realPower = 0;
unsigned long time = millis();

void setup() {
  Serial.begin(115200);
  Vex5.begin(BAUDRATE, SPI_NSS_PIN, IS_RESET_SHIELD);
  motor.begin(VEX5_PORT_1);
  time = millis();
}

void loop() {
  motor.setSpeed(goalSpeed);
  motor.getSpeed(realSpeed);
  motor.getPosition(realPosition);
  delay(10);
  motor.getPower(realPower);
  Serial.print("Goal Speed:\t" + String(goalSpeed) + "\t");
  Serial.println("Real Position Speed Power:\t" + String(realPosition) + "\t" + String(realSpeed) + "\t" + String(realPower));
  if(millis() - time > 1000) {
    time = millis();
    goalSpeed = -goalSpeed;
  }
  delay(300);
}