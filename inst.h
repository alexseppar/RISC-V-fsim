#ifndef INST_H
#define INST_H

#include <cassert>
#include <cstdint>
#include <vector>

enum class OpType
{
    INVALID,
    REG,
    IMM
};

class GenInst;

class Operand final
{
private:
    OpType m_type;

    union
    {
        uint8_t m_reg;

        uint32_t m_imm;
    };

    friend GenInst;

    Operand(OpType type, uint32_t val) : m_type(type)
    {
        if (type == OpType::REG)
        {
            m_reg = static_cast<uint8_t>(val);
        }
        else if (type == OpType::IMM)
        {
            m_imm = val;
        }
    }

public:
    Operand() = delete;

    OpType
    GetType() const
    {
        return m_type;
    }

    uint32_t
    GetImm() const
    {
        assert(m_type == OpType::IMM);
        return m_imm;
    }

    uint8_t
    GetRegNum() const
    {
        assert(m_type == OpType::REG);
        return m_reg;
    }
};

enum class InstName
{
    INVALID,
    ADD,
    SUB,
    OR,
    XOR,
    AND,
    SLT,
    SLTU,
    SRA,
    SRL,
    SLL,
    MUL,
    /*ADDI,
    SUBI,
    XORI,
    ANDI,
    SLTI,
    SLTIU,
    SRAI,
    SRLI,
    SLLI,
    LUI*/
};

enum class InstFormat
{
    INVALID,
    RT,
    IT,
    ST,
    UT
};

class Inst final
{
private:
    InstName m_name;

    InstFormat m_format;

    std::vector<Operand> m_operands;

    friend GenInst;

public:
    InstName
    GetName() const
    {
        return m_name;
    }

    InstFormat
    GetFormat() const
    {
        return m_format;
    }

    const Operand &
    GetOperand(uint8_t idx) const
    {
        return m_operands.at(idx);
    }
};

class GenInst final
{
    Inst *m_inst;

public:
    operator Inst *()
    {
        return m_inst;
    }

    GenInst(InstName name, InstFormat format)
    {
        // What should we do if name or format is INVALID?
        assert(format != InstFormat::INVALID);
        assert(name != InstName::INVALID);
        m_inst           = new Inst;
        m_inst->m_name   = name;
        m_inst->m_format = format;
    }

    GenInst &
    L(uint32_t imm)
    {
        m_inst->m_operands.push_back(Operand(OpType::IMM, imm));
        return *this;
    }

    GenInst &
    R(uint8_t reg)
    {
        assert(reg < 32);
        m_inst->m_operands.push_back(Operand(OpType::REG, reg));
        return *this;
    }

    ~GenInst()
    {
    }
};

#endif
