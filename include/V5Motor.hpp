#ifndef V5_MOTOR_HPP
#define V5_MOTOR_HPP

#include "motor.hpp"
#include "Vex5.h"

namespace V5::motor {

util::Error failedMotorConnectionError(util::ErrorCode::failedConnection, "Seems like connection with the Vex5 motor via shield failed");

class V5MotorController : public base::motor::controllers::RangedSpeedController,
public base::motor::controllers::InitializationController<VEX5_PORT_t> {

protected:
    Vex5_Motor motor;
    
    virtual util::Error setSpeedRaw(base::motor::Speed speed) {
        if(motor.setSpeed(speed) == -1) return failedMotorConnectionError;
        return util::Error();
    }
public:
    
    using RangedSpeedController::RangedSpeedController;
    
    virtual util::Error init(VEX5_PORT_t port) {
        if (port < 0 || port > 12) 
            return util::Error(util::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]");
        if (motor.begin(port) == -1) 
            return failedMotorConnectionError;
        return util::Error();
    }
    
    virtual util::Result<base::motor::Speed> getSpeed() const {
        // base::motor::Speed speed;
        // if(motor.getSpeed(speed) == -1) return failedMotorConnectionError;
        return motor.getSpeed();
    }
};

}

#endif