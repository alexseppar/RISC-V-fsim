#include "sim.h"

namespace sim
{
// State
void State::Dump(FILE *f) const
{
    fprintf(f, "Processor state:\n");
    for (uint8_t i = 0; i < 32; ++i)
    {
        ir::Reg(i).Dump(f);
        fprintf(f, ": 0x%08X%c", regs_[i], (i + 1) % 8 ? '\t' : '\n');
    }
    fprintf(f, "PC: 0x%08X\n", pc_);
}

// Trace
Trace::Trace(const Decoder &decoder, State &state)
{
    uint32_t address = state.GetPC();
    while (true)
    {
        ir::Inst inst = decoder.Decode(state.GetCmd(address));
        trace_.push_back(inst);
        isa::Opcode opcode = isa::GetCmdDesc(trace_.back().GetCmd()).opcode;
        if (opcode == isa::Opcode::BRANCH || opcode == isa::Opcode::JALR ||
            opcode == isa::Opcode::JAL || inst.GetCmd() == isa::Cmd::FENCE)
            break;
        address += 4;
    }
    trace_.shrink_to_fit();
    if (options::verbose)
    {
        fprintf(options::log, "Fetching trace:\n");
        Dump(options::log);
    }
}

void Trace::Dump(FILE *f) const
{
    for (const auto &inst : trace_)
        inst.Dump(f);
}

// TraceCache
void TraceCache::Dump(FILE *f) const
{
    fprintf(f, "Cached traces:\n");
    for (const auto &trace : cache_)
    {
        fprintf(f, "Address: 0x%#08X\n", trace.first);
        trace.second.Dump(f);
    }
}

// Sim
Sim::Sim(const std::vector<std::vector<uint32_t>> &commands,
         const std::vector<uint32_t> &seg_va,
         uint32_t pc)
    : state_(commands, seg_va, pc)
{
}

void Sim::Execute()
{
    Timer timer;
    timer.Start();
    try
    {
        while (true)
        {
            state_.trace_cache.Refer(decoder_, state_, state_.GetPC()).Execute(&state_);
            if (state_.GetExecutedInsts() >= options::max_insts)
                break;
        }
    }
    catch (SimException &e)
    {
        fprintf(options::log, "%s\n", e.what());
    }
    timer.Finish();
    state_.Dump(options::log);
    fprintf(options::log, "Some statistics:\n");
    uint64_t exec_insts = state_.GetExecutedInsts();
    uint64_t time = timer.GetMilliseconds();
    fprintf(options::log, "Number of instructions executed: %lu, time: %lu ms, MIPS: %.3lf\n",
            exec_insts, time, (double)exec_insts / (time * 1000));
    fprintf(options::log, "Trace cache: hits: %lu, misses: %lu\n",
            state_.trace_cache.GetHits(), state_.trace_cache.GetMisses());
}
}   // namespace sim
