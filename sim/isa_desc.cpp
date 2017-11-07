#include "isa_desc.h"
#include "common.h"
#include <cassert>

namespace isa
{
// clang-format off
const OpcodeDesc opcode_desc[] =
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

const CmdDesc cmd_desc[] =
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
    {"SLLI",   &ExecDummy, Opcode::OP_IMM, 0b001},
    {"SRLI",   &ExecDummy, Opcode::OP_IMM, 0b101},
    {"SRAI",   &ExecDummy, Opcode::OP_IMM, 0b101},
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

size_t GetOpcodesNum()
{
    return ArrSize(opcode_desc);
}

const OpcodeDesc &GetOpcodeDesc(uint8_t opcode)
{
    assert(opcode < GetOpcodesNum());
    return opcode_desc[opcode];
}

const OpcodeDesc &GetOpcodeDesc(Opcode opcode)
{
    return GetOpcodeDesc(static_cast<uint8_t>(opcode));
}

size_t GetCmdsNum()
{
    return ArrSize(cmd_desc);
}

const CmdDesc &GetCmdDesc(uint8_t cmd)
{
    assert(cmd < GetCmdsNum());
    return cmd_desc[cmd];
}

const CmdDesc &GetCmdDesc(Cmd cmd)
{
    return GetCmdDesc(static_cast<uint8_t>(cmd));
}
}
