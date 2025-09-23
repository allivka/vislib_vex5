#include"Vex5.h"
int led_pin = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
  
  Serial.begin(115200);
  Serial.println("To update firmware please enter bootloader:");
  Serial.println("1) Hold POWER button");
  Serial.println("2) Press and release RESET button");
  Serial.println("3) Wait for updating completion");
  Serial.println("4) (Optional) You can release POWER button when updating starts");
  Serial.println("Start updating");

  Vex5.begin_bootloader(500000, 39);
  digitalWrite(led_pin, LOW);

  int res = Vex5.updateFirmware();
  
  if(res >= 0)
  {
    Serial.println("Firmware updated");
  }
  else
  {
    Serial.println("ERROR " + String(res));
    while(1)
    {
        digitalWrite(led_pin, LOW);
        delay(250);
        digitalWrite(led_pin, HIGH);
        delay(250);
    };
  }
  digitalWrite(led_pin, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
}