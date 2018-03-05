#ifndef SIM_H
#define SIM_H

#include "common.h"
#include "decoder.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <vector>

namespace sim
{
class State
{
private:
    std::array<uint32_t, 32> regs_;
    uint32_t pc_;
    uint64_t executed_insts_;
    // TODO: system registers, MMU
    std::vector<uint32_t> commands_;   // use MMU in future
public:
    State(const std::vector<uint32_t> commands)
        : pc_(0)
        , executed_insts_(0)
        , commands_(commands)
    {
        regs_.fill(0u);
    }
    State(const std::vector<uint32_t> commands, uint32_t pc)
        : pc_(pc)
        , executed_insts_(0)
        , commands_(commands)
    {
        regs_.fill(0u);
    }
    uint32_t GetReg(ir::Reg reg) const
    {
        assert(reg < 32 && "Invalid register number");
        return regs_[reg];
    }
    void SetReg(ir::Reg reg, uint32_t val)
    {
        assert(reg < 32 && "Invalid register number");
        if (reg)
        {
            if (options::verbose)
            {
                reg.Dump(options::log);
                fprintf(options::log, ": 0x%08X => 0x%08X\n", regs_[reg], val);
            }
            regs_[reg] = val;
        }
    }

    uint32_t GetPC() const
    {
        return pc_;
    }
    void SetPC(uint32_t pc)
    {
        if (options::verbose)
            fprintf(options::log, "PC: 0x%08X => 0x%08X\n", pc_, pc);
        pc_ = pc;
    }

    uint64_t GetExecutedInsts()
    {
        return executed_insts_;
    }
    void AddExecutedInsts(uint32_t num)
    {
        executed_insts_ += num;
    }
    void Dump(FILE *f) const;

    // use MMU in future
    uint32_t GetCmd(uint32_t index) const
    {
        return commands_[index];
    }
};

class Trace
{
private:
    std::vector<ir::Inst> trace_;

public:
    Trace(const Decoder &decoder, const State &state);
    void Execute(State *state) const
    {
        if (options::verbose)
            fprintf(options::log, "Executing trace:\n");
        trace_.data()->Exec(trace_.data(), state);
    }
    void Dump(FILE *f) const;
};

class TraceCache
{
private:
    LRUCache<uint32_t, Trace> cache_;
    uint64_t hits_, misses_;

public:
    TraceCache(size_t size)
        : cache_(size)
        , hits_(0)
        , misses_(0)
    {
    }
    const Trace &Refer(const Decoder &decoder, const State &state)
    {
        auto res = cache_.Insert(state.GetPC(), decoder, state);
        if (res.second)
            ++misses_;
        else
            ++hits_;
        return res.first;
    }
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
    Decoder decoder_;
    TraceCache trace_cache_;
    State state_;

public:
    Sim(const std::vector<uint32_t> &commands);
    Sim(const std::vector<uint32_t> &commands, uint32_t pc);
    Sim(const Sim &rhs) = delete;
    Sim &operator=(const Sim &rhs) = delete;
    void Execute();
};
}

#endif
