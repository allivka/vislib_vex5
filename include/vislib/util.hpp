#ifndef UTIL_HPP
#define UTIL_HPP

#include "types.hpp"
#include <math.h>

namespace vislib::util {

template <typename T> T abs(T x) {
    if(x < 0) return x * -1;
    return x;
}

template <typename T> uc_t sign(T x) {
    if(x < 0) return -1;
    if(x > 0) return 1;
    return 0;
}

double cosDegrees(double angle) {
    return cos(angle * M_PI / 180);
}

double sinDegrees(double angle) {
    return sin(angle * M_PI / 180);
}

double deg2Rad(double angle) {
    return angle * M_PI / 180;
}

double rad2Deg(double angle) {
    return angle * 180 / M_PI;
}

template <typename T> class Range {

public:
    T lowest = 0;
    T highest = 0;
    
    template<typename D> static D map(D x, D in_min, D in_max, D out_min, D out_max) {
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
        if (v < lowest) {
            return lowest;
        }
        
        if (v > highest) {
            return highest;
        }
        
        return v;
    }
    
    T mapValueFromRange(T v, Range<T> r) const {
        return map(v, r, *this);
    }
    
    T mapValueToRange(T v, Range<T> r) const {
        return map(v, *this, r);
    }
    
};

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

template<typename T, size_t SIZE> class DefinedArray {
private:
    T data[SIZE];

public:
    
    DefinedArray() {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = T();
        }
    }
    
    template<size_t N> DefinedArray(const T (&p_data)[N]) {
        static_assert(N == SIZE, "Array size must match DefinedArray size");
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = p_data[i];
        }
    }
    
    DefinedArray(const T (&p_data)[SIZE]) {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = p_data[i];
        }
    }
    
    DefinedArray(const DefinedArray<T, SIZE>& other) {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = other.data[i];
        }
    }
    
    DefinedArray(DefinedArray<T, SIZE>&& other) = default;
    
    DefinedArray<T, SIZE>& operator=(DefinedArray<T, SIZE>&& other) = default;
    
    DefinedArray<T, SIZE>& operator=(const DefinedArray<T, SIZE>& other) {
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
    
    Result<T&> at(size_t index) {
        if (index >= SIZE) {
            throw Error(ErrorCode::indexOutOfRange, "Index out of range in 'defined array' element access");
        }
        return data[index];
    }
    
    Result<const T&> at(size_t index) const {
        if (index >= SIZE) {
            throw Error(ErrorCode::indexOutOfRange, "Index out of range in 'defined array' element access");
        }
        return data[index];
    }
    
    constexpr size_t size() const { return SIZE; }
    constexpr bool empty() const { return SIZE == 0; }
    
};
template<typename T> class Array {
protected:
    size_t size = 0;
    T *data = nullptr;
    
public:
    
    Array() = default;

    Array(size_t p_size) : size(p_size), data(new T[size]) { }
    
    template<size_t N> explicit Array(const T (&p_data)[N]) : size(N), data(new T[size]) {
        for(size_t i = 0; i < size; i++) {
            data[i] = p_data[i];
        }
    }
    
    Array(const T p_data[], size_t p_size) : size(p_size), data(new T[size]) {
        for(size_t i = 0; i < size; i++) {
            data[i] = p_data[i];
        }
    }
    
    Array(const Array<T>& other) : size(0), data(nullptr) {
        if (other.size > 0 && other.data != nullptr) {
            data = new T[other.size];
            if (data) {
                size = other.size;
                for(size_t i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            }
        }
    }
    
    Array(Array<T>&& other) : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
    }
    
    ~Array() {
        delete [] data;
    }
    
    Array<T>& operator=(const Array<T>& other) {
        if (this == &other) return *this;
        
        T* newData = nullptr;
        size_t newSize = 0;
        
        if (other.size > 0 && other.data != nullptr) {
            newData = new T[other.size];
            if (newData) {
                newSize = other.size;
                for(size_t i = 0; i < newSize; i++) {
                    newData[i] = other.data[i];
                }
            }
        }
        
        delete[] data;
        data = newData;
        size = newSize;
        
        return *this;
    }
    
    Array<T>& operator=(Array<T>&& other) {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    bool operator==(const Array<T>& other) const {
        if (size != other.size) return false;
        if(data == other.data && data == nullptr) {
            return true;
        }
        if((data == nullptr || other.data == nullptr) && data != other.data) return false;
        
        for(size_t i = 0; i < size; i++) {
            if (data[i] != other.data[i]) return false;
        }
        
        return true;
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    Result<T&> at(size_t index) {
        if (data == nullptr) {
            return Error(ErrorCode::emptyArray, "could not access data of an empty array");
        }
        
        if (index >= size) {
            return Error(ErrorCode::indexOutOfRange, "index out of range in array element access");
        }
        
        return data[index];
    }
    
    Result<const T&> at(size_t index) const {
        if (data == nullptr) {
            return Error(ErrorCode::emptyArray, "could not access data of an empty array");
        }
        
        if (index >= size) {
            return Error(ErrorCode::indexOutOfRange, "index out of range in array element access");
        }
        return data[index];
    }
    
    size_t Size() const { return size; }
    bool empty() const { return size == 0; }
    void clear() {
        delete [] data;
        size = 0;
        data = nullptr;
    }
    
    T* Data() {
        return data;
    }
    
    const T* Data() const {
        return data;
    }
    
    Array<T> operator+(const Array<T>& other) const {
        Array<T> newArr;
        newArr.size = size + other.size;
        newArr.data = new T[newArr.size];
        for(size_t i = 0; i < size; i++) {
            newArr.data[i] = data[i];
        }
        
        for(size_t i = 0; i < other.size; i++) {
            newArr.data[size + i] = other.data[i];
        }
        
        return newArr;
    }
    
};
class String : public Array<char> {
private:
    static size_t strlen(const char* str) {
        if (!str) return 0;
        const char* ptr = str;
        while (*ptr) ptr++;
        return ptr - str;
    }

public:
    String() : Array<char>() {}
    
    String(const char* cstr) : Array<char>(cstr, strlen(cstr)) {}
    
    String(const char* cstr, size_t length) : Array<char>(cstr, length) {}
    
    String(size_t count, char ch) : Array<char>(count) {
        for (size_t i = 0; i < count; i++) {
            (*this)[i] = ch;
        }
    }
    
    using Array<char>::Array;
    using Array<char>::operator=;
    
    const char* c_str() const {
        if(this->empty()) return "";
        
        if(data[size - 1] != '\0') {
            
            char *result = new char[size + 1];
            
            for (size_t i = 0; i < size; i++) {
                result[i] = data[i];
            }
            
            result[size] = '\0';
            
            return result;
            
        }
        
        return Data();
    }
    
    size_t length() const {
        return Size();
    }
    
    String operator+(const String& other) const {
        String result;
        Array<char>& base = result;
        base = Array<char>::operator+(other);
        return result;
    }
    
    String operator+(const char* other) const {
        String temp(other);
        return *this + temp;
    }
    
    String& operator+=(const String& other) {
        *this = *this + other;
        return *this;
    }
    
    String& operator+=(const char* other) {
        *this = *this + other;
        return *this;
    }
    
    bool operator==(const String& other) const {
        return Array<char>::operator==(other);
    }
    
    bool operator==(const char* other) const {
        size_t other_len = strlen(other);
        if (Size() != other_len) return false;
        
        for (size_t i = 0; i < Size(); i++) {
            if ((*this)[i] != other[i]) return false;
        }
        return true;
    }
    
    bool operator!=(const String& other) const {
        return !(*this == other);
    }
    
    bool operator!=(const char* other) const {
        return !(*this == other);
    }
};

inline bool operator==(const char* lhs, const String& rhs) {
    return rhs == lhs;
}

inline bool operator!=(const char* lhs, const String& rhs) {
    return !(rhs == lhs);
}

inline String operator+(const char* lhs, const String& rhs) {
    String temp(lhs);
    return temp + rhs;
}

template<typename T> class UniquePtr {
protected:
    T *ptr = nullptr;
    
public:
    UniquePtr() = default;
    
    UniquePtr(const T& v) {
        ptr = new T(v);
    }
    
    UniquePtr(T *p_ptr) : ptr(p_ptr) {}
    
    UniquePtr(const UniquePtr&) = delete;
    
    UniquePtr(UniquePtr&& other) {
        ptr = other.ptr;
        other.ptr = nullptr;
    }
    
    ~UniquePtr() {
        delete ptr;
    }
    
    T& operator*() const {
        return *ptr;
    }
    
    T* operator->() const {
        return ptr;
    }
    
    explicit operator bool() const {
        return ptr != nullptr;
    }
    
    UniquePtr<T>& operator=(const UniquePtr&) = delete;
    
    UniquePtr<T>& operator=(UniquePtr&& other) {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }
    
    UniquePtr<T>& operator=(const T& v) {
        if(ptr == nullptr) {
            ptr = new T(v);
        } else {
            *ptr = v;
        }
        return *this;
    }
    
    T* get() const {
        return ptr;
    }
    
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
    
    void reset(T *new_ptr = nullptr) {
        delete ptr;
        ptr = new_ptr;
    }
    
};

class Error {
public:
    ErrorCode errcode;
    String msg = "Success";
    
    operator ErrorCode() {
        return errcode;
    }
    
    operator ll_t() {
        return static_cast<ll_t>(errcode);
    }
    
    operator String() {
        return msg;
    }
    
    operator const char*() {
        return msg.c_str();
    }
    
    explicit operator bool() {
        return errcode != ErrorCode::success;
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

    operator T() const {
        return value;
    }

    operator E() const {
        return err;
    }
    
    operator bool() const {
        return errorFlag;
    }

    T operator()() {
        return value;
    }

    T Value() const {
        return value;
    }

    E Err() const {
        return err;
    }
};

template <typename T> class Result : public ReturnResult<T, Error> {
public:
    Result(T v) : ReturnResult<T, Error>(v) {}
    Result(Error e) : ReturnResult<T, Error>(e) {}
};

}

#endif