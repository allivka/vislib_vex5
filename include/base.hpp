#ifndef BASE_HPP
#define BASE_HPP

#include <arduinoLib.hpp>

namespace robot::base::motor {

using AnglePosition = int;
using DistancePosition = int;
using Speed = int;

class SpeedRange {
public:
    Speed lowest = 0;
    Speed highest = 0;
    
    SpeedRange() = default;
    SpeedRange(Speed p_lowest, Speed p_highest) : lowest(p_lowest), highest(p_highest) {}
};

class MotorInfo {
public:
    AnglePosition angle = 0;
    DistancePosition distance = 0;
    SpeedRange speedRange;
    
    MotorInfo(AnglePosition p_a, DistancePosition p_d, SpeedRange p_speed) : angle(p_a), distance(p_d), speedRange(p_speed) {}
    
    Speed FitSpeed2Range(Speed speed, SpeedRange range) {
        return arduino::map(speed, range.lowest, range.highest, speedRange.lowest, speedRange.highest);
    }
};

class MotorController {
public:
    virtual void setSpeed() = 0;
};

}

#endif