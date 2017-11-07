#include "sim.h"
#include "common.h"
#include <iomanip>

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
    // if (options::LogTrace)
    //    std::cerr << reg << ": " << val << std::endl;
}

uint32_t State::GetPC() const
{
    return pc_;
}

void State::SetPC(uint32_t pc)
{
    // there we can implement observation of hot code for JIT purposes
    pc_ = pc;
}

void State::Dump(std::ostream &ostream) const
{
    SaveOstreamFlags save_flags(ostream);
    ostream << "Processor state:" << std::endl;
    ostream << std::setfill('0') << std::setw(8) << std::right << std::hex;
    for (uint8_t i = 1; i < 32; ++i)
    {
        ostream << ir::Reg(i);
        ostream << ": ";
        ostream << regs_[i] << std::endl;
    }
    ostream << "PC: " << pc_ << std::endl;
}

std::ostream &operator<<(std::ostream &ostream, const State &state)
{
    state.Dump(ostream);
    return ostream;
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
            std::cout << "Command: " << std::setw(8) << std::setfill('0') << std::hex
                      << std::right << cmd << std::endl;
            std::cout << "Instruction:" << std::endl << inst << std::endl;
            std::cout << "Execution:" << std::endl;
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
