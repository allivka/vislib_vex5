#ifndef UTIL_HPP
#define UTIL_HPP

#include <Arduino.h>

namespace vislib::util {

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

class Exception {
protected:
    String msg;
public:
    Exception() = default;
    Exception(const String& p) : msg(p) {}
    Exception(const Exception&) = default;
    Exception(Exception&&) = default;
    
    String what() {
        return msg;
    }
};

namespace exceptions {
    class IndexOutOfRange : public Exception {
    public:
        using Exception::Exception;
        String what() {
            return "Failed accessing array element at index out of range: " + msg;
        }
    };
}

template<typename T, size_t SIZE>
class Array {
private:
    T data[SIZE];

public:
    
    Array() {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = T();
        }
    }
    
    Array(const T (&p_data)[SIZE]) {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = p_data[i];
        }
    }
    
    Array(const Array<T, SIZE>& other) {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = other.data[i];
        }
    }
    
    Array<T, SIZE>& operator=(const Array<T, SIZE>& other) {
        if (this != &other) {
            for(size_t i = 0; i < SIZE; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    T& at(size_t index) {
        if (index >= SIZE) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (index >= SIZE) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    constexpr size_t size() const { return SIZE; }
    bool empty() const { return SIZE == 0; }
    
};

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
    
    Error() : errcode(ErrorCode::success), msg("Successful operation") {}
    
    Error(ErrorCode code, String p_msg = "Default error") : errcode(code), msg(p_msg) {
        if (code == ErrorCode::success) msg = "Successful operation";
        else if(p_msg == "Default error") msg = "Undefined error occur";
    }
    
};

template <typename T, typename E> class ReturnResult {
protected:
    
    bool errorFlag = false;

    T value;
    E err;

public:
    
    ReturnResult(T v) : errorFlag(false), value(v) { }

    ReturnResult(E e) : errorFlag(true), err(e) { }
    
    bool isError() const {
        return errorFlag;
    }

    T Value() const {
        if (!errorFlag) return value;
        return T();
    }

    Error Err() const {
        if (errorFlag) return err;
        return E();
    }
};

template <typename T> class Result : public ReturnResult<T, Error> {
public:
    Result(T v) :ReturnResult<T, Error>::ReturnResult(v) {}
    Result(Error e) : ReturnResult<T, Error>::ReturnResult(e) {}
};

}

#endif