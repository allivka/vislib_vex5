#ifndef BASE_HPP
#define BASE_HPP

#include <arduinoLib.hpp>

namespace robot::base::motor {

using AnglePosition = int;
using DistancePosition = int;
using Speed = int;

class MotorInfo {
public:
    AnglePosition angle;
    DistancePosition distance;
    Speed lowSpeed;
    Speed highSpeed;
    
    MotorInfo(AnglePosition p_a, DistancePosition p_d, Speed lowLimit, Speed highLimit) : angle(p_a), distance(p_d), lowSpeed(lowLimit), highSpeed(highLimit) {}
    
    Speed FitSpeed2Range(Speed speed, Speed lowestSpeed, Speed highestSpeed) {
        return arduino::map(speed, lowestSpeed, highestSpeed, lowSpeed, highSpeed);
    }
};

}

#endif