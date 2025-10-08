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
    
    class NullptrAccess : public Exception {
    public:
        using Exception::Exception;
        String what() {
            return "Failed accessing aobject at nullptr address: " + msg;
        }
    };
}

template<typename T> class Array {
private:
    T *data = nullptr;
    size_t size = 0;
public:
    
    Array(size_t p_size) : size(p_size) {
        data = new T[size];
        for(size_t i = 0; i < size; i++) {
            data[i] = T();
        }
    }
    
    Array(const T p_data[], size_t p_size) : size(p_size), data(new T[size]) {
        for(size_t i = 0; i < size; i++) {
            data[i] = p_data[i];
        }
    }
    
    Array(const Array<T>& other) : size(other.size), data(new T[size]) {
        
        for(size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
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
        
        T *newData = new T[other.size];
        
        for(size_t i = 0; i < other.size; i++) {
            newData[i] = other.data[i];
        }
        
        delete [] data;
        data = newData;
        size = other.size;
        
        return *this;
        
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    T& at(size_t index) {
        if (data == nullptr) {
            throw exceptions::NullptrAccess("could not access data of array as it was cleared");
        }
        
        if (index >= size) {
            throw exceptions::IndexOutOfRange();
        }
        
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (data == nullptr) {
            throw exceptions::NullptrAccess("could not access data of array as it was cleared");
        }
        
        if (index >= size) {
            throw exceptions::IndexOutOfRange();
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
    
};

template<typename T, size_t SIZE> class DefinedArray {
private:
    T data[SIZE];

public:
    
    DefinedArray() {
        for(size_t i = 0; i < SIZE; i++) {
            data[i] = T();
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
    
    T& at(size_t index) {
        if (index >= SIZE) {
            throw exceptions::IndexOutOfRange("Index out of range");
        }
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (index >= SIZE) {
            throw exceptions::IndexOutOfRange("Index out of range");
        }
        return data[index];
    }
    
    constexpr size_t size() const { return SIZE; }
    constexpr bool empty() const { return SIZE == 0; }
    
};

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
        *ptr = v;
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
    
    explicit virtual operator bool() {
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