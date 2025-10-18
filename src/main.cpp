#include "visioner.hpp"

using namespace vislib;

motor::SpeedRange rpmSpeedRange(-600, 600);
motor::SpeedRange motorUseSpeedRange(-1500, 1500);
motor::SpeedRange motorInterfaceAngularSpeedRange(
    motorUseSpeedRange.mapValueToRange(-1500, rpmSpeedRange) * 2 * PI, motorUseSpeedRange.mapValueToRange(1500, rpmSpeedRange) * 2 * PI);

double wheelR = 0.1;
double motorDistance = 0.3;

platform::PlatformMotorConfig config({
    motor::MotorInfo(-90, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(0, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(90, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange),
    motor::MotorInfo(180, motorDistance, wheelR, motorUseSpeedRange, motorInterfaceAngularSpeedRange)
});

platform::Platform<V5::motor::V5MotorController> plat(config);



void setup() {  
    Vex5.begin();
    plat.init(util::Array<VEX5_PORT_t>({(VEX5_PORT_t)1, (VEX5_PORT_t)2, (VEX5_PORT_t)3, (VEX5_PORT_t)4}));
    Serial.begin(9600);
    delay(5000);
    
}

void go(double angle, double speed) {
    auto speeds = platform::calculators::calculatePlatformLinearSpeeds(config, angle, speed);
    if(speeds) {
        Serial.println(("Ooops, something went wrong in calculating speeds for linear movement of the platform: " + speeds.Err().msg).c_str());
        return;
    }
    
    auto err = plat.setSpeeds(speeds);
    
    if(err) {
        Serial.println(("Ooops, something went wrong in applying speeds to motors for linear movement of the platform: " + err.msg).c_str());
    }
}

void move(double angle, double speed, ull_t delayMs) {
    go(angle, speed);
    delay(delayMs);
}

double speed = 1000;
ull_t sectionTime = 2000;

void loop() {
    move(0, speed, sectionTime);
    move(90, speed, sectionTime);
    move(180, speed, sectionTime);
    move(-90, speed, sectionTime);
}