#pragma once

#include <math.h>

namespace vislib::util {

template <typename T> T abs(T x) {
    if (x < (T)0) return x * (T)-1;
    return x;
}

template <typename T> int sign(T x) {
    if (x < (T)0) return -1;
    if (x > (T)0) return 1;
    return 0;
}

double cosDegrees(double angle) {
    return cos(angle * M_PI / 180.0);
}

double sinDegrees(double angle) {
    return sin(angle * M_PI / 180.0);
}

double deg2Rad(double angle) {
    return angle * M_PI / 180.0;
}

double rad2Deg(double angle) {
    return angle * 180.0 / M_PI;
}

template <typename T> class Range {
public:
    T lowest = 0;
    T highest = 0;

    template<typename D> static D map(D x, D in_min, D in_max, D out_min, D out_max) {
        if (in_max == in_min) {
            return out_min;
        }
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    template<typename D> static D map(D x, const Range<D>& in, const Range<D>& out) {
        return map(x, in.lowest, in.highest, out.lowest, out.highest);
    }

    Range() = default;
    Range(const Range&) = default;
    Range(T p_lowest, T p_highest) : lowest(p_lowest), highest(p_highest) {}

    bool contains(T v) const {
        return v >= lowest && v <= highest;
    }

    T restrict(T v) const {
        if (v < lowest) return lowest;
        if (v > highest) return highest;
        return v;
    }

    T mapValueFromRange(T v, Range<T> r) const {
        return map(v, r, *this);
    }

    T mapValueToRange(T v, Range<T> r) const {
        return map(v, *this, r);
    }

};

} //namespace vislib::util