#pragma once

#include <vislib.hpp>
#include <Vex5.h>

namespace V5::motor {

[[nodiscard]] static vislib::util::Error failedMotorConnectionError() noexcept {
    return {vislib::util::ErrorCode::failedConnection,
            "Connection with the Vex5 motor via shield failed"};
}

class V5MotorController : public vislib::motor::controllers::RangedSpeedController,
public vislib::motor::controllers::InitializationController<VEX5_PORT_t> {

protected:
    Vex5_Motor motor;
    
    [[nodiscard]] virtual vislib::util::Error setSpeedRaw(vislib::motor::Speed speed) noexcept override {
        if(motor.setSpeed(speed) == -1) return failedMotorConnectionError();
        return vislib::util::Error();
    }
    
    [[nodiscard]] virtual vislib::util::Result<vislib::motor::Speed> getSpeedRaw() const noexcept override {
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
    
    [[nodiscard]] virtual vislib::util::Error init(VEX5_PORT_t port) noexcept override {
        if (port < 1 || port > 12) 
            return {vislib::util::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]"};
        if (motor.begin(port) == -1) 
            return failedMotorConnectionError();
        return vislib::util::ErrorCode::success;
    }
    
};

} //namespace V5::motor
