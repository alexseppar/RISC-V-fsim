#ifndef ISA_DESC_H
#define ISA_DESC_H

#include "common.h"
#include "exec_inst.h"
#include <cstdint>

namespace isa
{
// clang-format off
// Command format description
enum class CmdFormat: uint8_t
{
    R, I, S, U, B, J
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
    uint32_t imm   :12;
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

// Opcode description
enum class Opcode: uint8_t
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
    LUI
};

struct OpcodeDesc
{
    uint8_t opcode;
    CmdFormat format;
};

constexpr OpcodeDesc opcode_desc[] =
{
    {0b00000, CmdFormat::I}, // LOAD
    {0b01000, CmdFormat::S}, // STORE
    {0b11000, CmdFormat::B}, // BRANCH
    {0b11001, CmdFormat::I}, // JALR
    {0b11011, CmdFormat::J}, // JAL
    {0b00100, CmdFormat::I}, // OP_IMM
    {0b01100, CmdFormat::R}, // OP
    {0b11100, CmdFormat::I}, // SYSTEM
    {0b00101, CmdFormat::U}, // AUIPC
    {0b01101, CmdFormat::U}  // LUI
};

// Commands description
enum class Cmd: uint8_t
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
    // TODO: ECALL, EBREAK, CSR insts
    // RV32M standard extension
    MUL,
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU
};

struct CmdDesc
{
    const char *name;
    ExecFunc exec_func;
    Opcode opcode;
    uint8_t funct3 = 0, funct7 = 0;
};

constexpr CmdDesc cmd_desc[] =
{
    {"LUI",    &ExecDummy, Opcode::LUI   },
    {"AUIPC",  &ExecDummy, Opcode::AUIPC },
    {"JAL",    &ExecDummy, Opcode::JAL   },
    {"JALR",   &ExecDummy, Opcode::JALR,   0b000},
    {"BEQ",    &ExecDummy, Opcode::BRANCH, 0b000},
    {"BNE",    &ExecDummy, Opcode::BRANCH, 0b001},
    {"BLT",    &ExecDummy, Opcode::BRANCH, 0b100},
    {"BGE",    &ExecDummy, Opcode::BRANCH, 0b101},
    {"BLTU",   &ExecDummy, Opcode::BRANCH, 0b110},
    {"BGEU",   &ExecDummy, Opcode::BRANCH, 0b111},
    {"LB",     &ExecDummy, Opcode::LOAD,   0b000},
    {"LH",     &ExecDummy, Opcode::LOAD,   0b001},
    {"LW",     &ExecDummy, Opcode::LOAD,   0b010},
    {"LBU",    &ExecDummy, Opcode::LOAD,   0b100},
    {"LHU",    &ExecDummy, Opcode::LOAD,   0b101},
    {"SB",     &ExecDummy, Opcode::STORE,  0b000},
    {"SH",     &ExecDummy, Opcode::STORE,  0b001},
    {"SW",     &ExecDummy, Opcode::STORE,  0b010},
    {"ADDI",   &ExecDummy, Opcode::OP_IMM, 0b000},
    {"SLTI",   &ExecDummy, Opcode::OP_IMM, 0b010},
    {"SLTIU",  &ExecDummy, Opcode::OP_IMM, 0b011},
    {"XORI",   &ExecDummy, Opcode::OP_IMM, 0b100},
    {"ORI",    &ExecDummy, Opcode::OP_IMM, 0b110},
    {"ANDI",   &ExecDummy, Opcode::OP_IMM, 0b111},
    {"SLLI",   &ExecDummy, Opcode::OP_IMM, 0b001}, //0b0000000},
    {"SRLI",   &ExecDummy, Opcode::OP_IMM, 0b101}, //0b0000000},
    {"SRAI",   &ExecDummy, Opcode::OP_IMM, 0b101}, //0b0100000},
    {"ADD",    &ExecDummy, Opcode::OP,     0b000, 0b0000000},
    {"SUB",    &ExecDummy, Opcode::OP,     0b000, 0b0100000},
    {"SLL",    &ExecDummy, Opcode::OP,     0b001, 0b0000000},
    {"SLT",    &ExecDummy, Opcode::OP,     0b010, 0b0000000},
    {"SLTU",   &ExecDummy, Opcode::OP,     0b011, 0b0000000},
    {"XOR",    &ExecDummy, Opcode::OP,     0b100, 0b0000000},
    {"SRL",    &ExecDummy, Opcode::OP,     0b101, 0b0000000},
    {"SRA",    &ExecDummy, Opcode::OP,     0b101, 0b0100000},
    {"OR",     &ExecDummy, Opcode::OP,     0b110, 0b0000000},
    {"AND",    &ExecDummy, Opcode::OP,     0b111, 0b0000000},
    // TODO: ECALL, EBREAK, CSR insts
    // RV32M standard extension
    {"MUL",    &ExecDummy, Opcode::OP,     0b000, 0b0000001},
    {"MULH",   &ExecDummy, Opcode::OP,     0b001, 0b0000001},
    {"MULHSU", &ExecDummy, Opcode::OP,     0b010, 0b0000001},
    {"MULHU",  &ExecDummy, Opcode::OP,     0b011, 0b0000001},
    {"DIV",    &ExecDummy, Opcode::OP,     0b100, 0b0000001},
    {"DIVU",   &ExecDummy, Opcode::OP,     0b101, 0b0000001},
    {"REM",    &ExecDummy, Opcode::OP,     0b110, 0b0000001},
    {"REMU",   &ExecDummy, Opcode::OP,     0b111, 0b0000001}
};
// clang-format on

constexpr isa::Opcode GetOpcode(isa::Cmd cmd)
{
    return isa::cmd_desc[static_cast<uint8_t>(cmd)].opcode;
}

constexpr isa::CmdFormat GetCmdFormat(isa::Cmd cmd)
{
    return isa::opcode_desc[static_cast<uint8_t>(GetOpcode(cmd))].format;
}
}

#endif
