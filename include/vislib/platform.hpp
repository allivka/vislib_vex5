#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "motor.hpp"

namespace vislib::platform {

using PlatformMotorConfig = util::Array<motor::MotorInfo>;
using PlatformMotorSpeeds = util::Array<motor::Speed>;

template<typename Controller> class Platform {
protected:
    util::Array<Controller> controllers;
    
public:
    Platform(PlatformMotorConfig configuration) {
        controllers = util::Array<Controller>(configuration.Size());
        for (size_t i = 0; i < controllers.Size(); i++) {
            controllers.at(i)() = Controller(configuration.at(i));
        }
    }
    
    util::Error setSpeeds(PlatformMotorSpeeds speeds) {
        if (speeds.Size() != controllers.Size()) {
            return util::Error(util::ErrorCode::invalidArgument, "Cannot apply speeds set to controller set as they have different sizes");
        }
        for(size_t i = 0; i < controllers.Size(); i++) {
            util::Error err = controllers.at(i)().setSpeed(speeds.at(i));
            if(err != util::ErrorCode::success) {
                err.msg = "Could not apply speed to motor controller, error encountered: " + err.msg;
                return err;
            }
        }
        
        return util::ErrorCode::success;
    }
    
    util::Error setSpeedsInRange(PlatformMotorSpeeds speeds, motor::SpeedRange range) {
        if (speeds.Size() != controllers.Size()) {
            return util::Error(util::ErrorCode::invalidArgument, "Cannot apply speeds set to controller set as they have different sizes");
        }
        for(size_t i = 0; i < controllers.Size(); i++) {
            util::Error err = controllers.at(i).setSpeedInRange(speeds.at(i), range);
            if(err != util::ErrorCode::success) {
                err.msg = "Could not apply speed to motor controller, error encountered: " + err.msg;
                return err;
            }
        }
    }
    
    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    template<typename C> void init(const util::Array<C>& ports) {
        for(size_t i = 0; i < controllers.Size(); i++) {
            controllers.at(i)().init(ports.at(i)());
        }
    }
};


namespace calculators {
    
    PlatformMotorSpeeds calculateLinearSpeed(PlatformMotorConfig conf) {
        
    }
}
    
}

#endif
