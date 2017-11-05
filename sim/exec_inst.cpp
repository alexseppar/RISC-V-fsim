#include "exec_inst.h"
#include "sim.h"
#include <iostream>

#define INC_PC() (state->SetPC(state->GetPC() + 4))
#define RS1 (state->GetReg(inst->GetRs1))
#define RS2 (state->GetReg(inst->GetRs2))
#define IMM (inst->GetImm())
#define RD(x) (state->SetReg(state->GetReg(inst->GetRd()), (x)))

void ExecDummy(const ir::Inst *inst, sim::State *state)
{
    std::cerr << "This instruction is not implemented yet" << std::endl;
}

/*
 void ExecADD (InstIR *inst, State *state)
 {
    RD(RS1 + RS2);
    INC_PC();
 }
 */
