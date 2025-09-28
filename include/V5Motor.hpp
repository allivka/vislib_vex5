#ifndef V5_MOTOR_HPP
#define V5_MOTOR_HPP

#include "motor.hpp"
#include "Vex5.h"

namespace V5::motor {


class V5MotorController : 
public base::motor::controllers::MotorInfoRequired, 
public base::motor::controllers::SpeedController,
public base::motor::controllers::InitializationController<VEX5_PORT_t> {

private:
    Vex5_Motor motor;
public:
    
    using MotorInfoRequired::MotorInfoRequired;
    
    util::Error init(VEX5_PORT_t port) {
        if (port < 0 || port > 12) 
            return util::Error(util::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]");
        if (motor.begin(port) == -1) 
            return util::Error(util::ErrorCode::failedConnection, "Could not connect to the Vex5 motor with shield");
        return util::Error();
    }
    
    // util::Error setSpeed(base::motor::Speed speed)

};

}

#endif