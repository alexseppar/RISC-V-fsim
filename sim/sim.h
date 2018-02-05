#ifndef SIM_H
#define SIM_H

#include "decoder.h"
#include <cstdint>
#include <cstdio>
#include <list>
#include <unordered_map>
#include <vector>

namespace sim
{
class State
{
private:
    uint32_t regs_[32];
    uint32_t pc_;
    uint64_t executed_insts_;
    // TODO: system registers
public:
    State()
        : pc_(0)
        , executed_insts_(0)
    {
    }
    uint32_t GetReg(ir::Reg reg) const
    {
        assert(reg < 32 && "Invalid register number");
        return (reg) ? regs_[reg] : 0u;
    }
    void SetReg(ir::Reg reg, uint32_t val);
    uint32_t GetPC() const
    {
        return pc_;
    }
    void SetPC(uint32_t pc);
    uint64_t GetExecutedInsts()
    {
        return executed_insts_;
    }
    void AddExecutedInsts(uint32_t num)
    {
        executed_insts_ += num;
    }
    void Dump(FILE *f) const;
};

// optimization of execution through handler chaining and caching of decoded instructions
class Trace
{
private:
    std::vector<ir::Inst> trace_;

public:
    Trace(uint32_t address, const Decoder &decoder, const std::vector<uint32_t> &commands);
    void Execute(State *state) const;
    void Dump(FILE *f) const;
};

// LRU cache of traces; currently indexed by virtual address (PC)
class TraceCache
{
private:
    typedef typename std::pair<uint32_t, Trace> addr_trace_t;
    typedef typename std::list<addr_trace_t>::iterator list_iter_t;
    std::list<addr_trace_t> traces_;
    std::unordered_map<uint32_t, list_iter_t> links_;
    size_t n_, size_;
    uint64_t hits_, misses_;

public:
    TraceCache(size_t size)
        : n_(0)
        , size_(size)
        , hits_(0)
        , misses_(0)
    {
    }
    const Trace &Refer(uint32_t address,
                       const Decoder &decoder,
                       const std::vector<uint32_t> &commands);
    uint64_t GetHits() const
    {
        return hits_;
    }
    uint64_t GetMisses() const
    {
        return misses_;
    }
    void Dump(FILE *f) const;
};

class Sim
{
private:
    State state_;
    Decoder decoder_;
    std::vector<uint32_t> commands_;   // TODO: use elf loader and place insts in memory
    TraceCache trace_cache_;

public:
    Sim(const std::vector<uint32_t> &commands);
    Sim(const Sim &rhs) = delete;
    Sim &operator=(const Sim &rhs) = delete;
    void Execute();
};
}

#endif
