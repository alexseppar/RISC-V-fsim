#include "sim.h"
#include "common.h"

namespace sim
{
// State
void State::SetReg(ir::Reg reg, uint32_t val)
{
    assert(reg < 32 && "Invalid register number");
    if (options::verbose)
    {
        reg.Dump(options::log);
        fprintf(options::log, ": 0x%08X => 0x%08X\n", regs_[reg], val);
    }
    regs_[reg] = val;
}

void State::SetPC(uint32_t pc)
{
    if (options::verbose)
        fprintf(options::log, "PC: 0x%08X => 0x%08X\n", pc_, pc);
    pc_ = pc;
}

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
Trace::Trace(uint32_t address, const Decoder &decoder, const std::vector<uint32_t> &commands)
{
    // TODO: in future, fetch of instructions should be done through MMU, and this process
    // can cause faults; now we simply read array of hard-coded instructions
    while (true)
    {
        ir::Inst inst = decoder.Decode(commands[address / 4]);
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

void Trace::Execute(State *state) const
{
    if (options::verbose)
        fprintf(options::log, "Executing trace:\n");
    trace_.data()->Exec(trace_.data(), state);
}

void Trace::Dump(FILE *f) const
{
    for (const auto &inst : trace_)
        inst.Dump(f);
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
        ++misses_;
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
        ++hits_;
        traces_.splice(traces_.begin(), traces_, it->second);
        it->second = traces_.begin();
    }
    return traces_.front().second;
}

void TraceCache::Dump(FILE *f) const
{
    fprintf(f, "Cached traces:\n");
    for (const auto &trace : traces_)
    {
        fprintf(f, "Address: 0x%#08X\n", trace.first);
        trace.second.Dump(f);
    }
}

// Sim
Sim::Sim(const std::vector<uint32_t> &commands)
    : commands_(commands)
    , trace_cache_(options::cache_size)
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
            trace_cache_.Refer(state_.GetPC(), decoder_, commands_).Execute(&state_);
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
