#include "exec_inst.h"
#include "sim.h"
#include <cstdint>
#include <cstdio>

#define RS1 ((int32_t)state->GetReg(cur_inst->GetRs1()))
#define RS2 ((int32_t)state->GetReg(cur_inst->GetRs2()))
#define IMM ((int32_t)cur_inst->GetImm())
#define GET_PC() (state->GetPC())
#define SET_PC(x) (state->SetPC(x))
#define RD(x) (state->SetReg(cur_inst->GetRd(), (x)))
#define NEXT_INST()                            \
    {                                          \
        cur_inst->Dump(stderr);                \
        (cur_inst + 1)->Exec(fst_inst, state); \
    }

void ExecDummy(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    fprintf(stderr, "This instruction is not implemented yet\n");
    NEXT_INST();
}

void ExecADD(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 + RS2);
    NEXT_INST();
}

void ExecADDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 + IMM);
    NEXT_INST();
}

void ExecJAL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    SET_PC((uint32_t)((int32_t)(GET_PC() + (cur_inst - fst_inst) * 4) + IMM * 2));
    cur_inst->Dump(stderr);
}
