#ifndef COMMON_H
#define COMMON_H

#include "options.h"
#include <chrono>
#include <exception>
#include <functional>
#include <list>
#include <unordered_map>

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
        return msg_;
    }
    virtual ~SimException() noexcept {}

private:
    const char *msg_;
};

// Timer, helpful for performance measurements
class Timer
{
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

private:
    std::chrono::high_resolution_clock::time_point t1_, t2_;
};

// LRU cache, helpful as trace cache (and TLB in future)
// ValType has to have default constructor
//
template<typename KeyType, typename ValType>
class LRUCache
{
public:
    typedef typename std::pair<KeyType, ValType> key_val_t;
    typedef typename std::list<key_val_t>::iterator list_iter_t;
    typedef typename std::list<key_val_t>::const_iterator list_const_iter_t;

    LRUCache(size_t size)
        : max_size_(size)
        , cur_size_(0)
    {
    }
    list_const_iter_t begin() const
    {
        return entries_.cbegin();
    }
    list_const_iter_t end() const
    {
        return entries_.cend();
    }
    template<typename... ValArgs>
    auto Insert(KeyType key, ValArgs &&... val_args)
    {
        typename std::unordered_map<KeyType, list_iter_t>::iterator it = links_.find(key);
        if (it == links_.end())
        {
            // not in cache
            if (cur_size_ < max_size_)
            {
                // have space
                ++cur_size_;
            }
            else
            {
                // cache is full
                links_.erase(entries_.back().first);
                entries_.pop_back();
            }
            // insert in front
            entries_.emplace_front(std::piecewise_construct, std::forward_as_tuple(key),
                                   std::forward_as_tuple(val_args...));
            links_.insert({key, entries_.begin()});
            return std::make_pair(std::ref(entries_.front().second), true);
        }
        else
        {
            // in cache
            entries_.splice(entries_.begin(), entries_, it->second);
            it->second = entries_.begin();
            return std::make_pair(std::ref(entries_.front().second), false);
        }
    }
    void Clear()
    {
        entries_.clear();
        links_.clear();
        cur_size_ = 0;
    }

private:
    std::list<key_val_t> entries_;
    std::unordered_map<KeyType, list_iter_t> links_;
    size_t max_size_, cur_size_;
};

template<typename... Args>
void log(const char *fmt, Args&&... args)
{
    if (options::verbose)
        fprintf(options::log, fmt, std::forward<Args>(args)...); //NOLINT
}

#endif
