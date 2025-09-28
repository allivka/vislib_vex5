#ifndef UTIL_HPP
#define UTIL_HPP

#include <Arduino.h>
#include <string.h>

namespace util {

template <typename T> class Range {
public:
    T lowest = 0;
    T highest = 0;
    
    Range() = default;
    Range(const Range&) = default;
    Range(T p_lowest, T p_highest) : lowest(p_lowest), highest(p_highest) {}
};

enum class ErrorCode {
    success,
    failure,
    invalidArgument
};

class Error {
public:
    ErrorCode errcode;
    String msg = "Success";
    
    virtual operator ErrorCode() {
        return errcode;
    }
    
    virtual operator int64_t() {
        return static_cast<int64_t>(errcode);
    }
    
    virtual operator String() {
        return msg;
    }
    
    virtual operator const char*() {
        return msg.c_str();
    }
    
    virtual bool operator==(const Error& err) const {
        return errcode == err.errcode;
    }
    
    Error(ErrorCode code, String p_msg = "Undefined error happened") : errcode(code), msg(p_msg) {}
};

template <typename T> class Result {
protected:
    union {
        Error err;
        T value;
    } data;

    bool errorFlag = false;
    
public:
    
    bool isError() const {
        return errorFlag;
    }
    
    Result(T p) : data(p) {}
    Result(Error e) : data(e) {
        errorFlag = true;
    }
    
    T Value() const {
        if(!errorFlag) return data.value;
        else return NULL;
    }
    
    T Err() const {
        if(errorFlag) return data.err;
        else return NULL;
    }
};

}
#endif