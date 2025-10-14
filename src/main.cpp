#include "visioner.hpp"

using namespace vislib;

motor::MotorInfo config[] = {motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180)),
motor::MotorInfo(0, 0, motor::SpeedRange(-1500, 1500), motor::AngularSpeedRange(-180, 180))};

platform::Platform<V5::motor::V5MotorController> plat(platform::PlatformMotorConfig(config, 4));

int ports[] = {1, 2, 3 ,4};

void setup() {  
    Vex5.begin();
    plat.init(util::Array<int>(ports, 4));
    
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