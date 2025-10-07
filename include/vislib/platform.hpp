#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "motor.hpp"

namespace vislib::platform {
template<size_t MOTORCOUNT> class PlatformInfo : public util::Array<motor::MotorInfo, MOTORCOUNT> {
    using util::Array<motor::MotorInfo, MOTORCOUNT>::Array;
    using util::Array<motor::MotorInfo, MOTORCOUNT>::operator=;
};

namespace controllers {

}
}

#endif
