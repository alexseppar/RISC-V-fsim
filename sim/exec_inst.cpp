#include "exec_inst.h"
#include "common.h"
#include "sim.h"
#include <cstdint>
#include <cstdio>

#define URS1 (state->GetReg(cur_inst->GetRs1()))
#define RS1 ((int32_t)state->GetReg(cur_inst->GetRs1()))
#define URS2 (state->GetReg(cur_inst->GetRs2()))
#define RS2 ((int32_t)state->GetReg(cur_inst->GetRs2()))
#define UIMM (cur_inst->GetImm())
#define IMM ((int32_t)cur_inst->GetImm())
#define GET_PC() (state->GetPC())
#define SET_PC(x) (state->SetPC(x))
#define RD(x) (state->SetReg(cur_inst->GetRd(), (x)))
#define NEXT_INST()                            \
    {                                          \
        if (options::verbose)                  \
            cur_inst->Dump(options::log);      \
        (cur_inst + 1)->Exec(fst_inst, state); \
    }
#define END_TRACE()                                       \
    {                                                     \
        if (options::verbose)                             \
            cur_inst->Dump(options::log);                 \
        state->AddExecutedInsts(cur_inst - fst_inst + 1); \
    }

void ExecDummy(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    fprintf(stderr, "This instruction is not implemented yet\n");
    NEXT_INST();
}

void ExecLUI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(UIMM << 12);
    NEXT_INST();
}

void ExecAUIPC(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(GET_PC() + (cur_inst - fst_inst) * 4 + (UIMM << 12));
    NEXT_INST();
}

void ExecJAL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    RD(cur_pc + 4);
    SET_PC(cur_pc + IMM * 2);
    END_TRACE();
}

void ExecJALR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    RD(cur_pc + 4);
    uint32_t offset = RS1 + IMM;
    SET_PC(cur_pc + (offset & !1ull));
    END_TRACE();
}

void ExecBEQ(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 == RS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
}

void ExecBNE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 != RS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
}

void ExecBLT(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 < RS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
}

void ExecBGE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 >= RS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
}

void ExecBLTU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(URS1 < URS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
}

void ExecBGEU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(URS1 >= URS2 ? cur_pc + IMM : cur_pc + 4);
    END_TRACE();
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
