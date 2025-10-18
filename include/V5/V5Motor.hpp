#ifndef V5_MOTOR_HPP
#define V5_MOTOR_HPP

#include <vislib/vislib.hpp>
#include "Vex5.h"

namespace V5::motor {

vislib::util::Error failedMotorConnectionError(vislib::util::ErrorCode::failedConnection, "Seems like connection with the Vex5 motor via shield failed");

class V5MotorController : public vislib::motor::controllers::RangedSpeedController,
public vislib::motor::controllers::InitializationController<VEX5_PORT_t> {

protected:
    Vex5_Motor motor;
    
    virtual vislib::util::Error setSpeedRaw(vislib::motor::Speed speed) override {
        if(motor.setSpeed(speed) == -1) return failedMotorConnectionError;
        return vislib::util::Error();
    }
    
public:
    
    using RangedSpeedController::RangedSpeedController;
    
    V5MotorController() = default;
    
    virtual vislib::util::Error init(VEX5_PORT_t port) {
        if (port < 0 || port > 12) 
            return vislib::util::Error(vislib::util::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]");
        if (motor.begin(port) == -1) 
            return failedMotorConnectionError;
        return vislib::util::Error();
    }
    
    virtual vislib::util::Result<vislib::motor::Speed> getSpeed() const {
        // base::motor::Speed speed;
        // if(motor.getSpeed(speed) == -1) return failedMotorConnectionError;
        return motor.getSpeed();
    }
};

}

#endif
