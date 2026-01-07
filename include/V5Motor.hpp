#pragma once

#include <vislib_arduino.hpp>
#include <Vex5.h>

namespace vislib::binds::vex5::motor {

[[nodiscard]] static vislib::core::Error failedMotorConnectionError() noexcept {
    return {core::ErrorCode::failedConnection,
            "Connection with the Vex5 motor via shield failed"};
}

class V5MotorController : public vislib::motor::controllers::RangedSpeedController,
public vislib::motor::controllers::InitializationController<VEX5_PORT_t> {

protected:
    Vex5_Motor motor;
    
    [[nodiscard]] virtual core::Error setSpeedRaw(vislib::motor::Speed speed) noexcept override {
        if(motor.setSpeed(speed) == -1) return failedMotorConnectionError();
        return core::Error();
    }
    
    [[nodiscard]] virtual vislib::core::Result<vislib::motor::Speed> getSpeedRaw() const noexcept override {
        int16_t speed;
        if(static_cast<Vex5_Motor>(motor).getSpeed(speed) == -1) return failedMotorConnectionError();
        return static_cast<vislib::motor::Speed>(speed);
    }
    
public:
    
    using RangedSpeedController::RangedSpeedController;
    
    V5MotorController() = default;
    V5MotorController(const V5MotorController&) = default;
    V5MotorController(V5MotorController&&) = default;
    V5MotorController& operator=(const V5MotorController&) = default;
    
    [[nodiscard]] virtual core::Error init(VEX5_PORT_t port) noexcept override {
        if (port < 1 || port > 12) 
            return {core::ErrorCode::invalidArgument, "Cannot initialize Vex5 motor controller for motor with port out of range [1, 12]"};
        if (motor.begin(port) == -1) 
            return failedMotorConnectionError();
        return core::ErrorCode::success;
    }
    
};

} //namespace V5::motor
