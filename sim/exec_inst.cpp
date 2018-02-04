#include "exec_inst.h"
#include "sim.h"
#include <cstdio>

#define INC_PC() (state->SetPC(state->GetPC() + 4))
#define RS1 (state->GetReg(inst->GetRs1))
#define RS2 (state->GetReg(inst->GetRs2))
#define IMM (inst->GetImm())
#define RD(x) (state->SetReg(state->GetReg(inst->GetRd()), (x)))

void ExecDummy(const ir::Inst *inst, sim::State *state)
{
    fprintf(stderr, "This instruction is not implemented yet\n");
}

/*
 void ExecADD (InstIR *inst, State *state)
 {
    RD(RS1 + RS2);
    INC_PC();
 }
 */
