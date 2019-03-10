#include "jit.h"

namespace jit
{

static asmjit::JitRuntime runtime;

void ExecTraceDeleter::operator()(ExecTracePtr trace)
{
    if (trace)
        runtime.release(trace);
}

ExecTracePtr TranslateTrace(const std::vector<ir::Inst> &trace)
{
    asmjit::CodeHolder code;
    code.init(runtime.getCodeInfo());
    Translator tr(&code);
    for (const auto &inst : trace)
        if (!inst.Translate(tr))
            return nullptr;
    ExecTracePtr func;
    asmjit::Error err = runtime.add(&func, &code);
    if (err)
        return nullptr;
    return func;
}
}   // namespace Jit
