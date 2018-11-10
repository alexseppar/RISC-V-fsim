#ifndef EXEC_INST_H
#define EXEC_INST_H

namespace ir
{
class Inst;
}

namespace sim
{
class State;
}

typedef void (*ExecFunc)(const ir::Inst *, const ir::Inst *, sim::State *);

void ExecDummy(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecECALL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecFENCE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecLB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecLW(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecLH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecLBU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecLHU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSW(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecLUI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecAUIPC(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecJAL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecJALR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBEQ(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBNE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBLT(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBGE(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBLTU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecBGEU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecADDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLTI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLTIU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecXORI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecORI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecANDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLLI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSRLI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSRAI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecADD(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSUB(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLT(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSLTU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecXOR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSRL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecSRA(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecOR(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecAND(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

void ExecCSRRW(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);

// RV32M standard extension
void ExecMUL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecMULH(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecMULHSU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecMULHU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecDIV(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecDIVU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecREM(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecREMU(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
#endif
