#ifndef TRANSLATE_INST_H
#define TRANSLATE_INST_H

namespace ir
{
class Inst;
}

namespace jit
{
class Translator;
}

typedef bool (*TranslateFunc)(jit::Translator&, const ir::Inst *);

bool TranslateUnsup(jit::Translator &tr, const ir::Inst *inst);

#endif
