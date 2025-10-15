#include "visioner.hpp"

using namespace vislib;

motor::MotorInfo config[] = {motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180))};

platform::Platform<V5::motor::V5MotorController> plat(platform::PlatformMotorConfig(config, 4));

VEX5_PORT_t ports[] = {(VEX5_PORT_t)1, (VEX5_PORT_t)2, (VEX5_PORT_t)3, (VEX5_PORT_t)4};

void setup() {  
    Vex5.begin();
    plat.init(util::Array<VEX5_PORT_t>(ports, 4));
    
}

int limit = 1500;

void loop() {
    for (int i = -limit; i <= limit; i += 10) {
        double speeds[] = {i, i, i, i};
        plat.setSpeeds(util::Array<double>(speeds, 4));
        delay(100);
    }
    for (int i = limit; i >= -limit; i -= 10) {
        double speeds[] = {i, i, i, i};
        plat.setSpeeds(util::Array<double>(speeds, 4));
        delay(100);
    }
}