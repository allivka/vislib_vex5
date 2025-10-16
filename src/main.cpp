#include "visioner.hpp"

using namespace vislib;

motor::MotorInfo config[] = {motor::MotorInfo(-90, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(90, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(180, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180))};

platform::Platform<V5::motor::V5MotorController> plat(platform::PlatformMotorConfig(config, 4));

VEX5_PORT_t ports[] = {(VEX5_PORT_t)1, (VEX5_PORT_t)2, (VEX5_PORT_t)3, (VEX5_PORT_t)4};

void setup() {  
    Vex5.begin();
    plat.init(util::Array<VEX5_PORT_t>(ports, 4));
    delay(5000);
    Serial.begin(9600);
}

int limit = 1500;

void go(double angle) {
    double speedAbs = 1000;
    
    double speeds[4];
    
    for(int i = 0; i < 4; i++) {
        speeds[i] = cos(angle - config[i].anglePos) * speedAbs;
        Serial.println(speeds[i]);
    }
    
    plat.setSpeeds(util::Array<double>(speeds, 4));
    delay(5000);
}

void loop() {
    go(0);
    go(180);
    
}