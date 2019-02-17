#include "jit.h"
#include "asmjit/asmjit.h"

namespace Jit
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
    asmjit::X86Assembler a(&code);
    for (const auto &inst : trace)
    {
        // TODO: emit code here
        return nullptr;
    }
    ExecTracePtr func;
    asmjit::Error err = runtime.add(&func, &code);
    if (err)
        return nullptr;
    return func;
}
}   // namespace Jit
