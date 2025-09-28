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
    
    bool contains(T v) const {
        return v >= lowest && v <= highest;
    }
    
    T restrict(T v) const {
        if (v < lowest) {
            return lowest;
        }
        
        if (v > highest) {
            return highest;
        }
        
        return v;
    }
    
    T mapValueFromRange(T v, Range<T> r) const {
        return map(v, r.lowest, r.highest, lowest, highest);
    }
    
};

int64_t fitInt64ToRange(int64_t value, Range<int64_t> original, Range<int64_t> target) {
    return map(value, original.lowest, original.highest, target.lowest, target.highest);
}

enum class ErrorCode {
    success,
    failure,
    invalidArgument,
    failedConnection
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
    
    Error(ErrorCode code = ErrorCode::success, String p_msg = "Default error") : errcode(code), msg(p_msg) {
        if (code == ErrorCode::success) msg = "Successful operation";
        else if(p_msg == "Default error") msg = "Undefined error occur";
    }
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