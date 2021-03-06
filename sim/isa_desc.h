#ifndef ISA_DESC_H
#define ISA_DESC_H

#include "exec_inst.h"
#include "translate_inst.h"
#include <cstdint>
#include <cstdlib>

namespace isa
{
// clang-format off
// Command format description
enum class CmdFormat: uint8_t
{
    R, I, S, U, B, J,
    // stub format
    UNDEFINED
};

// Declare only base RISC-V instruction formats
// B-type is equal to S-type, J-type is equal to U-type
struct RFormat
{
    uint8_t sz     :2;
    uint8_t opcode :5;
    uint8_t rd     :5;
    uint8_t funct3 :3;
    uint8_t rs1    :5;
    uint8_t rs2    :5;
    uint8_t funct7 :7;
}__attribute__((packed));

struct IFormat
{
    uint8_t sz     :2;
    uint8_t opcode :5;
    uint8_t rd     :5;
    uint8_t funct3 :3;
    uint8_t rs1    :5;
    uint16_t imm   :12;
}__attribute__((packed));

struct SFormat
{
    uint8_t sz     :2;
    uint8_t opcode :5;
    uint8_t imm1   :5;
    uint8_t funct3 :3;
    uint8_t rs1    :5;
    uint8_t rs2    :5;
    uint8_t imm2   :7;
}__attribute__((packed));

struct UFormat
{
    uint8_t sz     :2;
    uint8_t opcode :5;
    uint8_t rd     :5;
    uint32_t imm   :20;
}__attribute__((packed));
// clang-format on

// Opcode description
enum class Opcode : uint8_t
{
    LOAD,
    STORE,
    BRANCH,
    JALR,
    JAL,
    OP_IMM,
    OP,
    SYSTEM,
    AUIPC,
    LUI,
    MSCMEM,
    // stub opcode
    UNDEFINED
};

struct OpcodeDesc
{
    uint8_t opcode;
    CmdFormat format;
};

// Commands description
enum class Cmd : uint8_t
{
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    ECALL,
    EBREAK,
    URET,
    SRET,
    MRET,
    WFI,
    FENCE,
    CSRRW,
    CSRRS,
    CSRRC,
    CSRRWI,
    CSRRSI,
    CSRRCI,
    // RV32M standard extension
    MUL,
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,
    // stub command
    UNDEFINED
};

// Regs description
enum class Regs : uint8_t
{
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,
    t1,
    t2,
    s0,
    s1,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    t3,
    t4,
    t5,
    t6
};

struct CmdDesc
{
    const char *name;
    ExecFunc exec_func;
    TranslateFunc translate_func;
    Opcode opcode;
    uint8_t funct3 = 0, funct7 = 0;
};

size_t GetOpcodesNum();
const OpcodeDesc &GetOpcodeDesc(uint8_t opcode);
const OpcodeDesc &GetOpcodeDesc(Opcode opcode);

size_t GetCmdsNum();
const CmdDesc &GetCmdDesc(uint8_t cmd);
const CmdDesc &GetCmdDesc(Cmd cmd);
}   // namespace isa

#endif
