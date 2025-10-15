#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "util.hpp"

namespace vislib::motor {

using Speed = double;
using AngularSpeed = double;

using SpeedRange = util::Range<Speed>;
using AngularSpeedRange = util::Range<double>;

class MotorInfo {
public:
    double anglePos = 0;
    double distance = 0;
    SpeedRange speedRange;
    AngularSpeedRange angularSpeedRange;
    bool isReversed = false;
    
    MotorInfo() = default;
    
    MotorInfo(int p_ap, int p_d, SpeedRange p_speed, AngularSpeedRange p_angSpeed, bool p_reversed = false)
    : anglePos(p_ap), distance(p_d), speedRange(p_speed), angularSpeedRange(p_angSpeed), isReversed(p_reversed) {}
    
};

namespace controllers {

class MotorInfoIncluded {
protected:
    MotorInfo info;
public:
    
    MotorInfoIncluded() = default;
    
    MotorInfoIncluded(const MotorInfo& p_info) : info(p_info) {}
    MotorInfo Info() {
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
public:
    using MotorInfoIncluded::MotorInfoIncluded;
    
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