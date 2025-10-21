#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "motor.hpp"
#include <stdlib.h>

namespace vislib::platform {

using PlatformMotorConfig = util::Array<motor::MotorInfo>;
using PlatformMotorSpeeds = util::Array<motor::Speed>;

PlatformMotorConfig updateParallelAxisesForMotors(PlatformMotorConfig config, size_t precision) {
    for(size_t i = 0; i < config.Size(); i++) {
        for(size_t j = 0; j < config.Size(); j++) {
            if(i == j) continue;
            
            if(j < i && config[j].parallelAxisesAmount != 1) {
                config[i].parallelAxisesAmount = config[j].parallelAxisesAmount;
                continue;
            }
            
            double diff = round(util::abs(config[i].anglePos - config[j].anglePos) * pow(10, precision));
            if(diff == 0 || diff == 180) {
                config[i].parallelAxisesAmount++;
            }
        }
    }
    
    for(size_t i = 0; i < config.Size(); i++) {
        if(config[i].parallelAxisesAmount != 1) config[i].parallelAxisesAmount--;
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
            controllers.at(i)() = Controller(configuration.at(i));
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
            
            if (controllers.at(i)) {
                return util::Error(util::ErrorCode::initFailed, 
                    "failed initializing one of the platform motors, invalid motor controller with index" 
                    + util::String(ultoa(i, nullptr, 10)) + ": " + controllers.at(i).Err().msg);
            }
            
            if (ports.at(i)) {
                return util::Error(util::ErrorCode::initFailed, 
                    "failed initializing one of the platform motors, invalid port array was given at index " 
                    + util::String(ultoa(i, nullptr, 10)) + " : " + ports.at(i).Err().msg);
            }
            
            auto e = controllers.at(i)().init(ports.at(i));
            
            if(e) {
                return util::Error(util::ErrorCode::initFailed, 
                    "failed initializing one of the platform motors, failed motor controller initialization at index "
                    + util::String(ultoa(i, nullptr, 10)) + " and port with value " + util::String(ultoa(ports.at(i)(), nullptr, 10)) + ": " + e.msg);
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
}
    
}

#endif
