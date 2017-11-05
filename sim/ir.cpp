#include "ir.h"
#include "common.h"
#include <iomanip>

namespace ir
{
// Reg
void Reg::Dump(std::ostream &ostream) const
{
    SaveOstreamFlags save_flags(ostream);
    ostream << 'x' << std::setfill('0') << std::setw(2) << std::right << std::dec
            << static_cast<uint32_t>(reg_);
}

std::ostream &operator<<(std::ostream &ostream, Reg reg)
{
    reg.Dump(ostream);
    return ostream;
}

// Imm
void Imm::Dump(std::ostream &ostream) const
{
    SaveOstreamFlags save_flags(ostream);
    ostream << "i:" << std::setfill('0') << std::setw(8) << std::right << std::hex
            << imm_;
}

std::ostream &operator<<(std::ostream &ostream, Imm imm)
{
    imm.Dump(ostream);
    return ostream;
}

// Inst
void Inst::Dump(std::ostream &ostream) const
{
    SaveOstreamFlags save_flags(ostream);
    ostream << std::setw(7) << std::left << std::setfill(' ')
            << isa::cmd_desc[static_cast<uint8_t>(cmd_)].name;
    switch (GetCmdFormat())
    {
    case isa::CmdFormat::R:
        ostream << rs1_ << ',' << rs2_ << "->" << rd_;
        break;
    case isa::CmdFormat::I:
        ostream << rs1_ << ',' << imm_ << "->" << rd_;
        break;
    case isa::CmdFormat::S:
    case isa::CmdFormat::B:
        ostream << rs1_ << ',' << rs2_ << ": " << imm_;
        break;
    case isa::CmdFormat::U:
    case isa::CmdFormat::J:
        ostream << imm_ << "->" << rd_;
        break;
    };
}

std::ostream &operator<<(std::ostream &ostream, const Inst &inst)
{
    inst.Dump(ostream);
    return ostream;
}
}
