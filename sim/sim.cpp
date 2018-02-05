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
    reg.Dump(log);
    fprintf(log, ": 0x%08X <-> ", regs_[reg]);
    regs_[reg] = val;
    fprintf(log, "0x%08X\n", regs_[reg]);
}

uint32_t State::GetPC() const
{
    return pc_;
}

void State::SetPC(uint32_t pc)
{
    fprintf(log, "PC: 0x%08X <-> ", pc_);
    pc_ = pc;
    fprintf(log, "0x%08X\n", pc_);
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

// Trace
Trace::Trace(uint32_t address, const Decoder &decoder, const std::vector<uint32_t> &commands)
{
    // TODO: in future, fetch of instructions should be done through MMU, and this process
    // can cause faults; now we simply read array of hard-coded instructions
    fprintf(log, "Fetching trace:\n");
    while (true)
    {
        ir::Inst inst = decoder.Decode(commands[address / 4]);
        trace_.push_back(inst);
        inst.Dump(log);
        isa::Opcode opcode = isa::GetCmdDesc(trace_.back().GetCmd()).opcode;
        if (opcode == isa::Opcode::BRANCH || opcode == isa::Opcode::JALR ||
            opcode == isa::Opcode::JAL)
            break;
        address += 4;
    }
    trace_.shrink_to_fit();
}

void Trace::Execute(State *state) const
{
    fprintf(log, "Executing trace:\n");
    trace_.data()->Exec(trace_.data(), state);
}

// TraceCache
const Trace &TraceCache::Refer(uint32_t address,
                               const Decoder &decoder,
                               const std::vector<uint32_t> &commands)
{
    std::unordered_map<uint32_t, list_iter_t>::iterator it = links_.find(address);
    if (it == links_.end())
    {
        // not in cache
        if (n_ < size_)
        {
            // have space
            ++n_;
        }
        else
        {
            // cache is full
            links_.erase(traces_.back().first);
            traces_.pop_back();
        }
        // insert in front
        traces_.emplace_front(std::piecewise_construct, std::forward_as_tuple(address),
                              std::forward_as_tuple(address, decoder, commands));
        links_.insert({address, traces_.begin()});
    }
    else
    {
        // in cache
        traces_.splice(traces_.begin(), traces_, it->second);
        it->second = traces_.begin();
    }
    return traces_.front().second;
}

// Sim
Sim::Sim(const std::vector<uint32_t> &commands)
    : commands_(commands)
    , trace_cache_(5)
{
}

void Sim::Execute()
{
    try
    {
        while (true)
        {
            trace_cache_.Refer(state_.GetPC(), decoder_, commands_).Execute(&state_);
        }
    }
    catch (SimException &e)
    {
        fprintf(log, "%s\n", e.what());
    }
}
}
