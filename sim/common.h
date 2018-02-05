#ifndef COMMON_H
#define COMMON_H

#include "options.h"
#include <chrono>
#include <cstdio>
#include <exception>
#include <string>

// Get array size, helpful for working with opcode_desc, cmd_desc, etc.
template<typename T, size_t N>
constexpr size_t ArrSize(T (&)[N])
{
    return N;
}

// Simulator exceptions, helpful to their distinction from other runtime errors
class SimException : public std::exception
{
public:
    SimException(const char *msg) noexcept
        : msg_(msg)
    {
    }
    virtual const char *what() const noexcept
    {
        return msg_.c_str();
    }
    virtual ~SimException() noexcept {}

private:
    std::string msg_;
};

// Timer, helpful for performance measurements
class Timer
{
private:
    std::chrono::high_resolution_clock::time_point t1_, t2_;

public:
    void Start()
    {
        t1_ = std::chrono::high_resolution_clock::now();
    }
    void Finish()
    {
        t2_ = std::chrono::high_resolution_clock::now();
    }
    uint64_t GetMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(t2_ - t1_).count();
    }
};

#endif
