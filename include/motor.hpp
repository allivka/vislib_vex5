#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "util.hpp"

namespace base::motor {

using AnglePosition = int;
using AngleDirection = int;
using DistancePosition = int;
using Speed = int;

using SpeedRange = util::Range<Speed>;

class MotorInfo {
public:
    AnglePosition anglePos = 0;
    AngleDirection angleDir = 0;
    DistancePosition distance = 0;
    SpeedRange speedRange;
    
    MotorInfo(AnglePosition p_ap, AngleDirection p_ad, DistancePosition p_d, SpeedRange p_speed)
    : anglePos(p_ap), angleDir(p_ad), distance(p_d), speedRange(p_speed) {}
    
    Speed FitSpeed2Range(Speed speed, SpeedRange range) {
        return map(speed, range.lowest, range.highest, speedRange.lowest, speedRange.highest);
    }
};

namespace controllers {

class MotorInfoRequired {
protected:
    const MotorInfo& info;
public:
    MotorInfoRequired(const MotorInfo& p_info) : info(p_info) {}
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

class RangedSpeedController : public MotorInfoRequired, public SpeedController {
protected:
    virtual util::Error setSpeedRaw(Speed) = 0;
public:
    
    RangedSpeedController(const MotorInfo& p_info) : MotorInfoRequired(p_info) {}

    virtual util::Error setSpeed(Speed speed) {
        return setSpeedRaw(info.speedRange.restrict(speed));
    }

    virtual bool inSpeedRange(Speed speed) const {
        return info.speedRange.contains(speed);
    }
    
    virtual util::Error setSpeedInRange(Speed speed, SpeedRange range) {
        if (!range.contains(speed)) return util::Error(util::ErrorCode::invalidArgument, "given speed is not in it's original range");
        return setSpeed(info.speedRange.mapValueFromRange(speed, range));
    }
};

}

}

#endif