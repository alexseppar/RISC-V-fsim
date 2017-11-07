#ifndef COMMON_H
#define COMMON_H

#include <exception>
#include <iostream>
#include <string>

// Get array size, helpful for working with opcode_desc, cmd_desc, etc.
template<typename T, size_t N>
constexpr size_t ArrSize(T (&)[N])
{
    return N;
}

// RAII class for saving ostream flags, helpful in Dump() methods
class SaveOstreamFlags
{
public:
    SaveOstreamFlags(std::ostream &ostream)
        : ostream_(ostream)
        , flags_(ostream.flags())
    {
    }
    SaveOstreamFlags(const SaveOstreamFlags &rhs) = delete;
    SaveOstreamFlags &operator=(const SaveOstreamFlags &rhs) = delete;
    ~SaveOstreamFlags()
    {
        ostream_.flags(flags_);
    }

private:
    std::ostream &ostream_;
    std::ios::fmtflags flags_;
};

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
