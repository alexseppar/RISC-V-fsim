#include "ir.h"
#include "common.h"

namespace ir
{
// Reg
void Reg::Dump(FILE *f) const
{
    fprintf(f, "x%02u", reg_);
}

// Imm
void Imm::Dump(FILE *f) const
{
    fprintf(f, "i:0x%08X", imm_);
}

// Inst
void Inst::Dump(FILE *f) const
{
    fprintf(f, "%-7s", isa::GetCmdDesc(cmd_).name);
    switch (GetCmdFormat())
    {
    case isa::CmdFormat::R:
        rs1_.Dump(f);
        fputc(',', f);
        rs2_.Dump(f);
        fputs("->", f);
        rd_.Dump(f);
        break;
    case isa::CmdFormat::I:
        if (cmd_ == isa::Cmd::ECALL || cmd_ == isa::Cmd::EBREAK)
            break;
        if (cmd_ == isa::Cmd::CSRRWI || cmd_ == isa::Cmd::CSRRSI || cmd_ == isa::Cmd::CSRRCI)
            Imm(rs1_).Dump(f);
        else
            rs1_.Dump(f);
        fputc(',', f);
        imm_.Dump(f);
        fputs("->", f);
        rd_.Dump(f);
        break;
    case isa::CmdFormat::S:
    case isa::CmdFormat::B:
        rs1_.Dump(f);
        fputc(',', f);
        rs2_.Dump(f);
        fputs(": ", f);
        imm_.Dump(f);
        break;
    case isa::CmdFormat::U:
    case isa::CmdFormat::J:
        imm_.Dump(f);
        fputs("->", f);
        rd_.Dump(f);
        break;
    case isa::CmdFormat::UNDEFINED:
        fputs("UNDEFINED", f);
    };
    fputc('\n', f);
}
}
