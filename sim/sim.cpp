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
Trace::Trace(const Decoder &decoder, const State &state)
{
    // TODO: in future, fetch of instructions should be done through MMU, and this process
    // can cause faults; now we simply read array of hard-coded instructions
    uint32_t address = state.GetPC();
    while (true)
    {
        ir::Inst inst = decoder.Decode(state.GetCmd(address / 4));
        trace_.push_back(inst);
        isa::Opcode opcode = isa::GetCmdDesc(trace_.back().GetCmd()).opcode;
        if (opcode == isa::Opcode::BRANCH || opcode == isa::Opcode::JALR ||
            opcode == isa::Opcode::JAL)
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
Sim::Sim(const std::vector<uint32_t> &commands)
    : trace_cache_(options::cache_size)
    , state_(commands)
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
            trace_cache_.Refer(decoder_, state_).Execute(&state_);
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
    fprintf(options::log, "Trace cache: hits: %lu, misses: %lu\n", trace_cache_.GetHits(),
            trace_cache_.GetMisses());
}
}
