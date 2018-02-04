#ifndef SIM_H
#define SIM_H

#include "decoder.h"
#include <cstdint>
#include <cstdio>
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
    uint32_t GetReg(ir::Reg reg) const;
    void SetReg(ir::Reg reg, uint32_t val);
    uint32_t GetPC() const;
    void SetPC(uint32_t pc);
    void Dump(FILE *f) const;
};

class Sim
{
private:
    State state_;
    Decoder decoder_;
    std::vector<uint32_t> commands_;   // TODO: use elf loader in future
public:
    Sim(const std::vector<uint32_t> &commands);
    Sim(const Sim &rhs) = delete;
    Sim &operator=(const Sim &rhs) = delete;
    void Execute();
};
}

#endif
