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

void ExecADD(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecADDI(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
void ExecJAL(const ir::Inst *fst_inst, const ir::Inst *cur_inst, sim::State *state);
#endif
