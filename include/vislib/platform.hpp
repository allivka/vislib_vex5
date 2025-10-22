#pragma once

#include "motor.hpp"
#include <stdlib.h>
#include <stdio.h>

namespace vislib::platform {

using PlatformMotorConfig = util::Array<motor::MotorInfo>;
using PlatformMotorSpeeds = util::Array<motor::Speed>;

PlatformMotorConfig updateParallelAxisesForMotors(PlatformMotorConfig config, size_t precision) {
    for(size_t i = 0; i < config.Size(); i++) {
        config[i].parallelAxisesAmount = 1;
    }
    
    for(size_t i = 0; i < config.Size(); i++) {
        for(size_t j = i + 1; j < config.Size(); j++) {
            double diff = round(abs(config[i].anglePos - config[j].anglePos) * pow(10, precision));
            if(diff == 0 || diff == 180) {
                config[i].parallelAxisesAmount++;
                config[j].parallelAxisesAmount++;
            }
        }
    }
    
    return config;
}

template<typename Controller> class Platform {
protected:
    util::Array<Controller> controllers;
    
public:

    Platform(PlatformMotorConfig configuration, size_t parallelismPrecision = 0) {
        configuration = updateParallelAxisesForMotors(configuration, parallelismPrecision);
        controllers = util::Array<Controller>(configuration.Size());
        for (size_t i = 0; i < controllers.Size(); i++) {
            Controller ctrl(configuration[i]);
            controllers[i] = ctrl;
        }
    }
    
    util::Error setSpeeds(PlatformMotorSpeeds speeds) {
        if (speeds.Size() != controllers.Size()) {
            return util::Error(util::ErrorCode::invalidArgument, "Cannot apply speeds set to controller set as there are different amount of them");
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
    
    util::Error setSpeedsInRanges(PlatformMotorSpeeds speeds, util::Array<motor::SpeedRange> ranges) {
        if (speeds.Size() != controllers.Size() || speeds.Size() != ranges.Size()) {
            return util::Error(util::ErrorCode::invalidArgument, 
                "Cannot apply speeds from different ranges set to controller set as there are different amounts of them");
        }
        
        for(size_t i = 0; i < controllers.Size(); i++) {
            util::Error err = controllers.at(i).setSpeedInRange(speeds.at(i), ranges[i]);
            if(err != util::ErrorCode::success) {
                err.msg = "Could not apply speed to motor controller, error encountered: " + err.msg;
                return err;
            }
        }
        
        return util::ErrorCode::success;
    }
    
    template<typename C> util::Error init(const util::Array<C>& ports) {
        
        for(size_t i = 0; i < controllers.Size(); i++) {
            
            auto t = controllers.at(i);
            auto p = ports.at(i);
            
            if (t.isError()) {
                return util::Error(util::ErrorCode::initFailed, 
                    "failed initializing one of the platform motors, invalid motor controller with index" 
                    + util::to_string(i) + ": " + t.Err().msg);
            }
            
            if (p.isError()) {
                return util::Error(util::ErrorCode::invalidArgument, 
                    "failed initializing one of the platform motors, invalid port array was given at index " 
                    + util::to_string(i) + " : " + p.Err().msg);
            }
            
            auto e = t().init(p());
            
            if(e) {
                return util::Error(util::ErrorCode::initFailed, 
                    "failed initializing one of the platform motors, failed motor controller initialization at index "
                    + util::to_string(i) + " and port with value " + util::to_string(static_cast<unsigned long long>(p())) + ": " + e.msg);
            }
        }
        
        return util::ErrorCode::success;
    }
    
    const util::Array<Controller>& Controllers() const {
        return controllers;
    }
    
};

namespace calculators {
    
    util::Result<motor::Speed> calculateMotorLinearSpeed(motor::MotorInfo info, double angle, motor::Speed speed) {
        if(info.parallelAxisesAmount == 0) {
            return util::Error(util::ErrorCode::invalidArgument, "amount of motors with parallel movement axises cannot be zero in motor config");
        }
        
        if(!info.interfaceSpeedRange.contains(speed)) {
            return util::Error(util::ErrorCode::outOfRange, "the given speed is not in the configured motor interface speed range");
        }
        
        return util::cosDegrees(angle - info.anglePos) * speed / info.parallelAxisesAmount / info.wheelR;
        
    }
    
    util::Result<PlatformMotorSpeeds> calculatePlatformLinearSpeeds(PlatformMotorConfig config, double angle, motor::Speed speed) {
        PlatformMotorSpeeds speeds(config.Size());
        
        for(size_t i = 0; i < speeds.Size(); i++) {
            
            util::Result<motor::Speed> t = calculateMotorLinearSpeed(config[i], angle, speed);
            if(t) return t.Err();
            
            speeds[i] = t;
        }
        
        return speeds;
    }
} // namespace vislib::platform::calculators

} //namespace vislib::platform

