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
    // TODO: system registers
public:
    State()
        : pc_(0)
    {
    }
    uint32_t GetReg(ir::Reg reg) const;
    void SetReg(ir::Reg reg, uint32_t val);
    uint32_t GetPC() const;
    void SetPC(uint32_t pc);
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

public:
    TraceCache(size_t size)
        : n_(0)
        , size_(size)
    {
    }
    const Trace &Refer(uint32_t address,
                       const Decoder &decoder,
                       const std::vector<uint32_t> &commands);
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
