#pragma once

namespace vislib::util {

enum class ErrorCode {
    success,
    failure,
    initFailed,
    invalidArgument,
    failedConnection,
    outOfRange,
    indexOutOfRange,
    emptyArray,
    zeroDivision
};

class Error;
template<typename T> class Result;

} //namespace vislib::util
