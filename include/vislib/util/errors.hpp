#pragma once

#include "containers.hpp"

namespace vislib::util {

class Error {
public:
    ErrorCode errcode;
    String msg = "Success";

    operator ErrorCode() const { return errcode; }

    operator long long() const { return static_cast<long long>(errcode); }

    operator String() const { return msg; }

    operator const char*() const { return msg.c_str(); }

    explicit operator bool() const { return errcode != ErrorCode::success; }

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

    bool isError() const { return errorFlag; }
    bool isOK() const { return !errorFlag; }

    bool getValue(T& out) const {
        if (!errorFlag) {
            out = value;
            return true;
        }
        return false;
    }

    bool getError(E& out) const {
        if (errorFlag) {
            out = err;
            return true;
        }
        return false;
    }

    operator T() const { return value; }
    operator E() const { return err; }

    operator bool() const { return errorFlag; }

    T operator()() const { return value; }

    T Value() const { return value; }

    E Err() const { return err; }
};

template <typename T> class Result : public ReturnResult<T, Error> {
public:
    Result(T v) : ReturnResult<T, Error>(v) {}
    Result(Error e) : ReturnResult<T, Error>(e) {}
};

template <typename T> class Result<T&> : public ReturnResult<T*, Error> {
public:
    Result(T& v) : ReturnResult<T*, Error>(&v) {}
    Result(Error e) : ReturnResult<T*, Error>(e) {}
    
    T& operator()() const { return *(this->value); }

    T& Value() const { return *(this->value); }
};


} //namespace vislib::util
