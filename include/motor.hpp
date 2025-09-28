#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "util.hpp"

namespace base::motor {

using AnglePosition = int;
using DistancePosition = int;
using Speed = int;

using SpeedRange = util::Range<Speed>;

class MotorInfo {
public:
    AnglePosition angle = 0;
    DistancePosition distance = 0;
    SpeedRange speedRange;
    
    MotorInfo(AnglePosition p_a, DistancePosition p_d, SpeedRange p_speed) : angle(p_a), distance(p_d), speedRange(p_speed) {}
    
    Speed FitSpeed2Range(Speed speed, SpeedRange range) {
        return map(speed, range.lowest, range.highest, speedRange.lowest, speedRange.highest);
    }
};

namespace controllers {

class MotorInfoRequired {
public:
    const MotorInfo& info;
    
    MotorInfoRequired(const MotorInfo& p_info) : info(p_info) {}
};

class SpeedController : MotorInfoRequired {
public:

    virtual void setSpeed(Speed) = 0;
    virtual Speed getSpeed() const = 0;
    virtual void stop(bool isHard) = 0;
    
    using MotorInfoRequired::MotorInfoRequired;
    
};

}

}

#endif