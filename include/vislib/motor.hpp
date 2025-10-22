#pragma once

#include "util/util.hpp"

namespace vislib::motor {

using Speed = double;

using SpeedRange = util::Range<Speed>;

class MotorInfo {
public:
    double anglePos = 0;
    double distance = 1;
    double wheelR = 1;
    SpeedRange speedRange;
    SpeedRange interfaceSpeedRange;
    bool isReversed = false;
    
    size_t parallelAxisesAmount = 1;
    
    MotorInfo() = default;
    
    MotorInfo(double p_ap, double p_d, double p_wr, SpeedRange p_speed, SpeedRange p_intSpeed, bool p_reversed = false)
    : anglePos(p_ap), distance(p_d), wheelR(p_wr), speedRange(p_speed), interfaceSpeedRange(p_intSpeed), isReversed(p_reversed) {}
    
};

namespace controllers {

class MotorInfoIncluded {
protected:
    MotorInfo info;
public:
    
    MotorInfoIncluded() = default;
    
    MotorInfoIncluded(const MotorInfo& p_info) : info(p_info) {}
    MotorInfo Info() const {
        return info;
    }
};

template <typename T> class InitializationController {
public:
    virtual util::Error init(T) = 0;
};
class SpeedController {
public:

    virtual util::Error setSpeed(Speed) = 0;
    virtual util::Result<Speed> getSpeed() const = 0;
    
};

class RangedSpeedController : public MotorInfoIncluded, public SpeedController {
protected:
    virtual util::Error setSpeedRaw(Speed) = 0;
    virtual util::Result<Speed> getSpeedRaw() const = 0;
public:
    using MotorInfoIncluded::MotorInfoIncluded;
    
    virtual util::Error setSpeed(Speed speed) {
        return setSpeedRaw(info.interfaceSpeedRange.mapValueToRange(info.interfaceSpeedRange.restrict(info.isReversed ? -speed : speed), info.speedRange));
    }
    
    virtual util::Result<Speed> getSpeed() const {
        util::Result<Speed> rawSpeed = getSpeedRaw();
        if(rawSpeed) return rawSpeed;
        
        Speed mapped = info.speedRange.mapValueToRange(rawSpeed(), info.interfaceSpeedRange);
        
        return info.isReversed ? -mapped : mapped;
    }

    virtual bool inSpeedRange(Speed speed) const {
        return info.interfaceSpeedRange.contains(speed);
    }
    
    virtual util::Error setSpeedInRange(Speed speed, SpeedRange range) {
        if (!range.contains(speed)) return util::Error(util::ErrorCode::invalidArgument, "given speed is not in it's stated range");
        return setSpeed(info.interfaceSpeedRange.mapValueFromRange(speed, range));
    }
};

} //namespace vislib::motor::controllers

} //namespace vislib::motor
