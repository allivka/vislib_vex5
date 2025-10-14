#include "visioner.hpp"


void setup() {  
    // Vex5.begin();
    // motor.init(VEX5_PORT_1);
    // Serial.begin(9600);
}

int limit = 1500;

void loop() {
    for (int i = -limit; i <= limit; i += 10) {
        platform.setSpeeds(i);
        delay(100);
    }
    for (int i = limit; i >= -limit; i -= 10) {
        platform.setSpeeds(i);
        delay(100);
    }
}