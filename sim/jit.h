#ifndef JIT_H
#define JIT_H
#include "ir.h"
#include "asmjit/asmjit.h"
#include <memory>
#include <vector>

namespace jit
{

typedef void(ExecTrace)(sim::State *);
typedef void (*ExecTracePtr)(sim::State *);

struct ExecTraceDeleter
{
    void operator()(ExecTracePtr);
};

class Translator
{
public:
    Translator(asmjit::CodeHolder *code)
        : x86asm_(code)
    {
    }
    // TBD: current intention is to use some form of register allocator for trace translation.
    // So, we should process liveness analysis (in TranslateTrace?),and pass this information
    // to the Translator class. It should contain current risc-v reg -> x86 reg mapping, and
    // provide interface for accessing operands (they can be in x86 register or in state memory).
#if 0
    asmjit::Operand GetOperand(ir::Reg) const;
    asmjit::Operand GetOperand(ir::Imm) const;
#endif
private:
    asmjit::X86Assembler x86asm_;
};

ExecTracePtr TranslateTrace(const std::vector<ir::Inst> &trace);
}   // namespace Jit

#endif
