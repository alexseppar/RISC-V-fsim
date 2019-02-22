#include "syscall.h"
#include "sim.h"
#include <cstdio>

namespace syscall
{
void ExecRead([[maybe_unused]] sim::State *state)
{
    printf("Read unsupported\n");
}

void ExecWrite([[maybe_unused]] sim::State *state)
{
    const uint32_t a0 = state->GetReg(ir::Reg(10));
    const uint32_t a1 = state->GetReg(ir::Reg(11));
    const uint32_t a2 = state->GetReg(ir::Reg(12));
    if (a0 == 0 || a0 == 1 || a0 == 2)   // stdin, stdout, stderr
    {
        for (uint32_t i = 0; i < a2; ++i)
            fputc(state->Read(a1 + i, 1), stdout);
    }
    else
    {
        printf("Write for fd = %u unsupported\n", a0);
    }
}

void ExecExit([[maybe_unused]] sim::State *state)
{
    throw SimException("Finished!\n");
}

void ExecBrk([[maybe_unused]] sim::State *state)
{
    printf("Brk unsupported\n");
}

void ExecSysCall(sim::State *state, SysCall value)
{
    switch (value)
    {
    case SysCall::read:
        ExecRead(state);
        break;
    case SysCall::write:
        ExecWrite(state);
        break;
    case SysCall::exit:
        ExecExit(state);
        break;
    case SysCall::brk:
        ExecBrk(state);
        break;
    default:
        printf("Unssupported syscall %u\n", static_cast<uint32_t>(value));
    }
}
}   // namespace syscall
