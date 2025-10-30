#pragma once

#include <vislib/vislib.hpp>
#include "Vex5.h"

namespace V5::motor {

static vislib::util::Error failedMotorConnectionError() {
    return vislib::util::Error(vislib::util::ErrorCode::failedConnection, 
                              "Seems like connection with the Vex5 motor via shield failed");
}

class V5MotorController : public vislib::motor::controllers::RangedSpeedController,
public vislib::motor::controllers::InitializationController<VEX5_PORT_t> {

protected:
    Vex5_Motor motor;
    
    virtual vislib::util::Error setSpeedRaw(vislib::motor::Speed speed) override {
        if(motor.setSpeed(speed) == -1) return failedMotorConnectionError();
        return vislib::util::Error();
    }
    
    virtual vislib::util::Result<vislib::motor::Speed> getSpeedRaw() const override {
        int16_t speed;
        if(motor.getSpeed(speed) == -1) return failedMotorConnectionError();
        return static_cast<vislib::motor::Speed>(speed);
    }
    
public:
    
    using RangedSpeedController::RangedSpeedController;
    
    V5MotorController() = default;
    V5MotorController(const V5MotorController&) = default;
    V5MotorController(V5MotorController&&) = default;
    V5MotorController& operator=(const V5MotorController&) = default;
    
    virtual vislib::util::Error init(VEX5_PORT_t port) {
        if (port < 1 || port > 12) 
            return vislib::util::Error(vislib::util::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]");
        if (motor.begin(port) == -1) 
            return failedMotorConnectionError();
        return vislib::util::Error();
    }
    
};

} //namespace V5::motor
