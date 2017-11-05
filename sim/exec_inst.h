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

typedef void (*ExecFunc)(const ir::Inst *, sim::State *);

void ExecDummy(const ir::Inst *inst, sim::State *state);
#endif
