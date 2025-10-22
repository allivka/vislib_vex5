#pragma once

#include "types.hpp"
#include "errordef.hpp"

namespace vislib::util {

template<typename T, size_t SIZE> class DefinedArray {
private:
    T data[SIZE];

public:

    DefinedArray() {
        for(size_t i = 0; i < SIZE; i++) data[i] = T();
    }

    template<size_t N> DefinedArray(const T (&p_data)[N]) {
        static_assert(N == SIZE, "Array size must match DefinedArray size");
        for(size_t i = 0; i < SIZE; i++) data[i] = p_data[i];
    }

    DefinedArray(const T (&p_data)[SIZE]) {
        for(size_t i = 0; i < SIZE; i++) data[i] = p_data[i];
    }

    DefinedArray(const DefinedArray<T, SIZE>& other) {
        for(size_t i = 0; i < SIZE; i++) data[i] = other.data[i];
    }

    DefinedArray(DefinedArray<T, SIZE>&& other) = default;
    DefinedArray<T, SIZE>& operator=(DefinedArray<T, SIZE>&& other) = default;

    DefinedArray<T, SIZE>& operator=(const DefinedArray<T, SIZE>& other) {
        if (this != &other) {
            for(size_t i = 0; i < SIZE; i++) data[i] = other.data[i];
        }
        return *this;
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    Result<T&> at(size_t index) {
        if (index >= SIZE) {
            return Error(ErrorCode::indexOutOfRange, "Index out of range in 'defined array' element access");
        }
        return data[index];
    }

    Result<const T&> at(size_t index) const {
        if (index >= SIZE) {
            return Error(ErrorCode::indexOutOfRange, "Index out of range in 'defined array' element access");
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
        for(size_t i = 0; i < size; i++) data[i] = p_data[i];
    }

    Array(const T p_data[], size_t p_size) : size(p_size), data(new T[size]) {
        for(size_t i = 0; i < size; i++) data[i] = p_data[i];
    }

    Array(const Array<T>& other) : size(0), data(nullptr) {
        if (other.size > 0 && other.data != nullptr) {
            data = new T[other.size];
            if (data) {
                size = other.size;
                for(size_t i = 0; i < size; i++) data[i] = other.data[i];
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
                for(size_t i = 0; i < newSize; i++) newData[i] = other.data[i];
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
        if(data == other.data && data == nullptr) return true;
        if((data == nullptr || other.data == nullptr) && data != other.data) return false;

        for(size_t i = 0; i < size; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

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

    T* Data() { return data; }
    const T* Data() const { return data; }

    Array<T> operator+(const Array<T>& other) const {
        Array<T> newArr;
        newArr.size = size + other.size;
        newArr.data = new T[newArr.size];
        for(size_t i = 0; i < size; i++) newArr.data[i] = data[i];
        for(size_t i = 0; i < other.size; i++) newArr.data[size + i] = other.data[i];
        return newArr;
    }

};

class String : public Array<char> {
private:
    static size_t c_strlen(const char* str) {
        if (!str) return 0;
        const char* ptr = str;
        while (*ptr) ptr++;
        return ptr - str;
    }

    void ensureNullTerminated() {
        if (this->Size() == 0) return;
        if (this->Data()[this->Size() - 1] != '\0') {
            this->Data()[this->Size() - 1] = '\0';
        }
    }

public:
    String() : Array<char>() {}

    String(const char* cstr) : Array<char>(c_strlen(cstr) + 1) {
        size_t len = c_strlen(cstr);
        for(size_t i = 0; i < len; i++) {
            data[i] = cstr[i];
        }
        data[len] = '\0';
    }

    String(const char* cstr, size_t length) : Array<char>(cstr, length) {}

    String(size_t count, char ch) : Array<char>(count) {
        for (size_t i = 0; i < count; i++) (*this)[i] = ch;
    }

    using Array<char>::Array;
    using Array<char>::operator=;

    const char* c_str() const {
        if (this->Size() == 0) return "";
        if (this->Data()[this->Size() - 1] == '\0') return Data();
        return Data();
    }

    size_t length() const {
        if (this->Size() == 0) return 0;
        if (this->Data()[this->Size() - 1] == '\0') return this->Size() - 1;
        return this->Size();
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
        size_t other_len = c_strlen(other);
        if (length() != other_len) return false;
        for (size_t i = 0; i < other_len; i++) {
            if ((*this)[i] != other[i]) return false;
        }
        return true;
    }

    bool operator!=(const String& other) const { return !(*this == other); }
    bool operator!=(const char* other) const { return !(*this == other); }
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

inline String to_string(unsigned long long value) {
    if (value == 0) return String("0");
    char buf[32];
    size_t idx = 0;
    unsigned long long v = value;
    while (v > 0 && idx < sizeof(buf)-1) {
        buf[idx++] = char('0' + (v % 10));
        v /= 10;
    }
    // reverse into a String
    String s(idx + 1, '\0'); // include space for NUL
    for (size_t i = 0; i < idx; i++) s[i] = buf[idx - 1 - i];
    s[idx] = '\0';
    return s;
}

inline String to_string(long long value) {
    if (value < 0) {
        unsigned long long uv = (unsigned long long)(-value);
        String s = String("-") + to_string(uv);
        return s;
    }
    return to_string((unsigned long long)value);
}

} //namespace vislib::util
