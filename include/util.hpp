#ifndef UTIL_HPP
#define UTIL_HPP

#include <Arduino.h>

namespace util {

template <typename T> class Range {
public:
    T lowest = 0;
    T highest = 0;
    
    Range() = default;
    Range(const Range&) = default;
    Range(T p_lowest, T p_highest) : lowest(p_lowest), highest(p_highest) {}
};

}
#endif