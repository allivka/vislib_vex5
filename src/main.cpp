#include "visioner.hpp"

using namespace vislib;

motor::SpeedRange rpmSpeedRange(-600, 600);
motor::SpeedRange motorUseSpeedRange(-1500, 1500);
motor::SpeedRange motorInterfaceAngularSpeedRange(motorUseSpeedRange.mapValueToRange(-1500, rpmSpeedRange) * 2 * PI, motorUseSpeedRange.mapValueToRange(1500, rpmSpeedRange) * 2 * PI);

double wheelR = 0.1;
double motorDistance = 0.3;

platform::Platform<V5::motor::V5MotorController> plat(platform::PlatformMotorConfig((motor::MotorInfo[]){
    motor::MotorInfo(-90, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(0, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(90, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(180, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange)
}, 4));

void setup() {  
    Vex5.begin();
    plat.init(util::Array<VEX5_PORT_t>((VEX5_PORT_t[]){(VEX5_PORT_t)1, (VEX5_PORT_t)2, (VEX5_PORT_t)3, (VEX5_PORT_t)4}, 4));
    Serial.begin(9600);
    delay(5000);
    
}


void go(double angle) {
    
}

void loop() {
    
}