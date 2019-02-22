#ifndef SYSCALL_H
#define SYSCALL_H

#include <cstdint>

namespace sim
{
class State;
}

namespace syscall
{
enum class SysCall : uint32_t
{
    read = 63,
    write = 64,
    exit = 93,
    brk = 214
};

void ExecSysCall(sim::State *state, SysCall value);
}   // namespace syscall
#endif
