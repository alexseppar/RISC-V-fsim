#include "exec_inst.h"
#include "common.h"
#include "sim.h"
#include <cstdint>
#include <cstdio>

#define SIGN_EXTEND(val, bitsUsed) (static_cast<int32_t>(static_cast<int##bitsUsed##_t>(val)))
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
    fprintf(options::log, "This instruction is not implemented yet\n");
    NEXT_INST();
}

void ExecLB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 1);
    RD(SIGN_EXTEND(val, 8));
    NEXT_INST();
}

void ExecLW(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 4);
    RD(SIGN_EXTEND(val, 32));
    NEXT_INST();
}

void ExecLH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 2);
    RD(SIGN_EXTEND(val, 16));
    NEXT_INST();
}

void ExecLBU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 1);
    RD(val);
    NEXT_INST();
}

void ExecLWU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 4);
    RD(val);
    NEXT_INST();
}

void ExecLHU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t val = state->Read(RS1 + IMM, 2);
    RD(val);
    NEXT_INST();
}

void ExecSB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    state->Write(RS1 + IMM, 1, RS2);
    NEXT_INST();
}

void ExecSW(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    state->Write(RS1 + IMM, 4, RS2);
    NEXT_INST();
}

void ExecSH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    state->Write(RS1 + IMM, 2, RS2);
    NEXT_INST();
}

void ExecLUI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(UIMM);
    NEXT_INST();
}

void ExecAUIPC(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(GET_PC() + (cur_inst - fst_inst) * 4 + UIMM);
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
    SET_PC(offset & ~1u);
    END_TRACE();
}

void ExecBEQ(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 == RS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecBNE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 != RS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecBLT(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 < RS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecBGE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(RS1 >= RS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecBLTU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(URS1 < URS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecBGEU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t cur_pc = GET_PC() + (cur_inst - fst_inst) * 4;
    SET_PC(URS1 >= URS2 ? cur_pc + IMM * 2 : cur_pc + 4);
    END_TRACE();
}

void ExecADDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 + IMM);
    NEXT_INST();
}

void ExecSLTI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 < IMM ? 1 : 0);
    NEXT_INST();
}

void ExecSLTIU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 < UIMM ? 1 : 0);
    NEXT_INST();
}

void ExecXORI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 ^ IMM);
    NEXT_INST();
}

void ExecORI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 | IMM);
    NEXT_INST();
}

void ExecANDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 & IMM);
    NEXT_INST();
}

void ExecSLLI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 << UIMM);
    NEXT_INST();
}

void ExecSRLI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 >> UIMM);
    NEXT_INST();
}

void ExecSRAI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint32_t mask = (RS1 < 0 && UIMM > 0) ? ~(~0u >> UIMM) : 0;
    RD((URS1 >> UIMM) | mask);
    NEXT_INST();
}

void ExecADD(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 + RS2);
    NEXT_INST();
}

void ExecSUB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 - RS2);
    NEXT_INST();
}

void ExecSLL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 << (URS2 & 0b11111));
    NEXT_INST();
}

void ExecSLT(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 < RS2 ? 1 : 0);
    NEXT_INST();
}

void ExecSLTU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 < URS2 ? 1 : 0);
    NEXT_INST();
}

void ExecXOR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 ^ RS2);
    NEXT_INST();
}

void ExecSRL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 >> (URS2 & 0b11111));
    NEXT_INST();
}

void ExecSRA(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint8_t offset = URS2 & 0b11111;
    uint32_t mask = (RS1 < 0 && offset > 0) ? ~(~0u >> offset) : 0;
    RD((URS1 >> offset) | mask);
    NEXT_INST();
}

void ExecOR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 | RS2);
    NEXT_INST();
}

void ExecAND(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(RS1 & RS2);
    NEXT_INST();
}

// RV32M standard extension
void ExecMUL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS1 * URS2);
    NEXT_INST();
}

void ExecMULH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint64_t result = (int64_t)RS1 * (int64_t)RS2;
    RD(result >> 32);
    NEXT_INST();
}

void ExecMULHSU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint64_t result = (int64_t)RS1 * (uint64_t)URS2;
    RD(result >> 32);
    NEXT_INST();
}

void ExecMULHU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    uint64_t result = (uint64_t)URS1 * (uint64_t)URS2;
    RD(result >> 32);
    NEXT_INST();
}

void ExecDIV(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    if (RS2 == 0)
        RD(-1);
    else if (RS2 == -1 && RS1 == INT32_MIN)
        RD(INT32_MIN);
    else
        RD(RS1 / RS2);
    NEXT_INST();
}

void ExecDIVU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS2 ? URS1 / URS2 : UINT32_MAX);
    NEXT_INST();
}

void ExecREM(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    if (RS2 == 0)
        RD(RS1);
    else if (RS2 == -1 && RS1 == INT32_MIN)
        RD(0);
    else
        RD(RS1 % RS2);
    NEXT_INST();
}

void ExecREMU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state)
{
    RD(URS2 ? URS1 % URS2 : URS1);
    NEXT_INST();
}
