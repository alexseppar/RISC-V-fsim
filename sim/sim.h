#ifndef SIM_H
#define SIM_H

#include "common.h"
#include "decoder.h"
#include "mmu.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

namespace sim
{
class State;

class Trace
{
private:
    std::vector<ir::Inst> trace_;

public:
    Trace(const Decoder &decoder, State &state);
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
    const Trace &Refer(const Decoder &decoder, State &state, uint32_t addr)
    {
        auto res = cache_.Insert(addr, decoder, state);
        if (res.second)
            ++misses_;
        else
            ++hits_;
        return res.first;
    }
    void Flush()
    {
        cache_.Clear();
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

class State
{
private:
    std::array<uint32_t, 32> regs_;
    uint32_t pc_;
    uint64_t executed_insts_;
    // TODO: system registers, MMU
    // std::vector<uint32_t> commands_;   // use MMU in future
    uint64_t pmem_size_ = 25 * 4096;
    uint8_t *pmem_;
    MMU mmu_;

public:
    TraceCache trace_cache;
    uint32_t satp = 0x00000017u;   // reg
    State(const std::vector<std::vector<uint32_t>> &commands,
          const std::vector<uint32_t> &seg_va,
          uint32_t pc)
        : pc_(pc)
        , executed_insts_(0)
        , pmem_(new uint8_t[pmem_size_]())
        , mmu_(pmem_, pmem_size_, satp)
        , trace_cache(options::cache_size)
    {
        regs_.fill(0u);
        regs_[2] = pmem_size_ - 2 * 4096;
        // put segment in pmem_ (pa = va)
        int i = 0;
        for (auto va : seg_va)
        {
            memcpy(pmem_ + va, commands[i].data(), commands[i].size() * 4);
            ++i;
        }
    }
    ~State()
    {
        delete[] pmem_;
    }
    uint32_t GetReg(ir::Reg reg) const
    {
        assert(reg < 32 && "Invalid register number");
        return regs_[reg];
    }
    void SetReg(ir::Reg reg, uint32_t val)
    {
        // assert(reg < 32 && "Invalid register number");
        if (reg)
        {
            // if (options::verbose)
            // {
            //     fprintf(options::log, "\t");
            //     reg.Dump(options::log);
            //     fprintf(options::log, ": 0x%08X => 0x%08X\n", regs_[reg], val);
            // }
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
            fprintf(options::log, "\tPC: 0x%08X => 0x%08X\n", pc_, pc);
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

    uint32_t GetCmd(uint32_t va)
    {
        return mmu_.Load(va, sizeof(va));
    }

    void Write(uint32_t va, uint8_t nbytes, uint32_t data)
    {
        if (options::verbose)
            fprintf(options::log, "\tM: 0x%08X <= 0x%08X\n", va,
                    nbytes == 4 ? data : data & ((1 << (8 * nbytes)) - 1));
        mmu_.Store(va, nbytes, data);
    }

    uint32_t Read(uint32_t va, uint8_t nbytes)
    {
        return mmu_.Load(va, nbytes, false);
    }

    void Flush()
    {
        trace_cache.Flush();
        mmu_.Flush();
    }
};

class Sim
{
private:
    Decoder decoder_;
    State state_;

public:
    Sim(const std::vector<uint32_t> &commands);
    Sim(const std::vector<std::vector<uint32_t>> &commands,
        const std::vector<uint32_t> &seg_va,
        uint32_t pc);
    Sim(const Sim &rhs) = delete;
    Sim &operator=(const Sim &rhs) = delete;
    void Execute();
};
}   // namespace sim

#endif
