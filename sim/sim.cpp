#include "sim.h"
#include "common.h"

namespace sim
{
// State
uint32_t State::GetReg(ir::Reg reg) const
{
    assert(reg < 32 && "Invalid register number");
    return (reg) ? regs_[reg] : 0u;
}

void State::SetReg(ir::Reg reg, uint32_t val)
{
    assert(reg < 32 && "Invalid register number");
    regs_[reg] = val;
}

uint32_t State::GetPC() const
{
    return pc_;
}

void State::SetPC(uint32_t pc)
{
    pc_ = pc;
}

void State::Dump(FILE *f) const
{
    fprintf(f, "Processor state:\n");
    for (uint8_t i = 0; i < 32; ++i)
    {
        ir::Reg(i).Dump(f);
        fprintf(f, ": 0x%08X\n", regs_[i]);
    }
    fprintf(f, "PC: 0x%08X\n", pc_);
}

// Sim
Sim::Sim(const std::vector<uint32_t> &commands)
    : commands_(commands)
{
}

void Sim::Execute()
{
    for (auto cmd : commands_)
    {
        try
        {
            ir::Inst inst = decoder_.Decode(cmd);
            fprintf(log, "Command: 0x%08X\n", cmd);
            fprintf(log, "Instruction:\n");
            inst.Dump(log);
            fprintf(log, "Execution:\n");
            (*isa::GetCmdDesc(inst.GetCmd()).exec_func)(&inst, &state_);
        }
        catch (SimException &e)
        {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
}
}
