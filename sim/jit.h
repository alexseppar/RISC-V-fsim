#ifndef JIT_H
#define JIT_H
#include "ir.h"
#include <memory>
#include <vector>

namespace Jit
{

typedef void(ExecTrace)(sim::State *);
typedef void (*ExecTracePtr)(sim::State *);

struct ExecTraceDeleter
{
    void operator()(ExecTracePtr trace);
};

ExecTracePtr TranslateTrace(const std::vector<ir::Inst> &trace);
}   // namespace Jit

#endif
