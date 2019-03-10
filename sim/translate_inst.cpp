#include "translate_inst.h"
#include "common.h"

bool TranslateUnsup([[maybe_unused]] jit::Translator &tr,
                    [[maybe_unused]] const ir::Inst *inst)
{
    log("Translation of this instruction is not implemented yet\n");
    return false;
}
