#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <exception>
#include <string>

// Logging stream
extern FILE *log;

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

#endif
