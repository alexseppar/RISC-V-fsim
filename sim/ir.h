#ifndef IR_H
#define IR_H

#include "exec_inst.h"
#include "isa_desc.h"
#include <cassert>
#include <cstdio>
#include <type_traits>

namespace ir
{
class Reg
{
private:
    uint8_t reg_;

public:
    Reg(uint8_t reg)
        : reg_(reg)
    {
        assert(reg_ < 32);
    }
    Reg(isa::Regs reg)
        : reg_(static_cast<uint8_t>(reg))
    {
        assert(reg_ < 32);
    }
    operator uint8_t() const
    {
        return reg_;
    }
    void Dump(FILE *f) const;
};

class Imm
{
private:
    uint32_t imm_;

public:
    Imm(uint32_t imm)
        : imm_(imm)
    {
    }
    operator uint32_t() const
    {
        return imm_;
    }
    void Dump(FILE *f) const;
};

class Inst
{
private:
    Imm imm_;
    Reg rs1_, rs2_, rd_;
    isa::Cmd cmd_;
    ExecFunc func_;

public:
    Inst(isa::Cmd cmd, Reg rd, Reg rs1, Reg rs2, Imm imm)
        : imm_(imm)
        , rs1_(rs1)
        , rs2_(rs2)
        , rd_(rd)
        , cmd_(cmd)
        , func_(isa::GetCmdDesc(cmd_).exec_func)
    {
    }
    bool IsImm() const
    {
        return GetCmdFormat() != isa::CmdFormat::R;
    }
    Imm GetImm() const
    {
        assert(IsImm());
        return imm_;
    }
    bool IsRs1() const
    {
        return GetCmdFormat() != isa::CmdFormat::U && GetCmdFormat() != isa::CmdFormat::J;
    }
    Reg GetRs1() const
    {
        assert(IsRs1());
        return rs1_;
    }
    bool IsRs2() const
    {
        return GetCmdFormat() != isa::CmdFormat::I && GetCmdFormat() != isa::CmdFormat::U &&
               GetCmdFormat() != isa::CmdFormat::J;
    }
    Reg GetRs2() const
    {
        assert(IsRs2());
        return rs2_;
    }
    bool IsRd() const
    {
        return GetCmdFormat() != isa::CmdFormat::S && GetCmdFormat() != isa::CmdFormat::B;
    }
    Reg GetRd() const
    {
        assert(IsRd());
        return rd_;
    }
    isa::Cmd GetCmd() const
    {
        return cmd_;
    }
    isa::Opcode GetOpcode() const
    {
        return isa::GetCmdDesc(cmd_).opcode;
        ;
    }
    isa::CmdFormat GetCmdFormat() const
    {
        return isa::GetOpcodeDesc(GetOpcode()).format;
    }
    void Dump(FILE *f) const;
    void Exec(const ir::Inst *fst_inst, sim::State *state) const
    {
        func_(fst_inst, this, state);
    }
    bool Translate(jit::Translator &tr) const
    {
        return isa::GetCmdDesc(cmd_).translate_func(tr, this);
    }
};

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::R, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Reg rd,
                                                                         Reg rs1,
                                                                         Reg rs2)
{
    Inst inst(cmd, rd, rs1, rs2, 0);
    assert(inst.GetCmdFormat() == isa::CmdFormat::R);
    return inst;
}

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::I, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Reg rd,
                                                                         Reg rs1,
                                                                         Imm imm)
{
    Inst inst(cmd, rd, rs1, 0, imm);
    assert(inst.GetCmdFormat() == isa::CmdFormat::I);
    return inst;
}

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::S, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Imm imm,
                                                                         Reg rs1,
                                                                         Reg rs2)
{
    Inst inst(cmd, 0, rs1, rs2, imm);
    assert(inst.GetCmdFormat() == isa::CmdFormat::S);
    return inst;
}

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::U, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Reg rd,
                                                                         Imm imm)
{
    Inst inst(cmd, rd, 0, 0, imm);
    assert(inst.GetCmdFormat() == isa::CmdFormat::U);
    return inst;
}

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::B, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Imm imm,
                                                                         Reg rs1,
                                                                         Reg rs2)
{
    Inst inst(cmd, 0, rs1, rs2, imm);
    assert(inst.GetCmdFormat() == isa::CmdFormat::B);
    return inst;
}

template<isa::CmdFormat Format>
typename std::enable_if<Format == isa::CmdFormat::J, Inst>::type GenInst(isa::Cmd cmd,
                                                                         Reg rd,
                                                                         Imm imm)
{
    Inst inst(cmd, rd, 0, 0, imm);
    assert(inst.GetCmdFormat() == isa::CmdFormat::J);
    return inst;
}
}   // namespace ir

#endif
