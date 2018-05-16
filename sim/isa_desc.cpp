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
    {"LUI",    &ExecLUI,   Opcode::LUI   },
    {"AUIPC",  &ExecAUIPC, Opcode::AUIPC },
    {"JAL",    &ExecJAL,   Opcode::JAL   },
    {"JALR",   &ExecJALR,  Opcode::JALR,   0b000},
    {"BEQ",    &ExecBEQ,   Opcode::BRANCH, 0b000},
    {"BNE",    &ExecBNE,   Opcode::BRANCH, 0b001},
    {"BLT",    &ExecBLT,   Opcode::BRANCH, 0b100},
    {"BGE",    &ExecBGE,   Opcode::BRANCH, 0b101},
    {"BLTU",   &ExecBLTU,  Opcode::BRANCH, 0b110},
    {"BGEU",   &ExecBGEU,  Opcode::BRANCH, 0b111},
    {"LB",     &ExecLB,    Opcode::LOAD,   0b000},
    {"LH",     &ExecLH,    Opcode::LOAD,   0b001},
    {"LW",     &ExecLW,    Opcode::LOAD,   0b010},
    {"LBU",    &ExecLBU,   Opcode::LOAD,   0b100},
    {"LHU",    &ExecLHU,   Opcode::LOAD,   0b101},
    {"SB",     &ExecSB,    Opcode::STORE,  0b000},
    {"SH",     &ExecSH,    Opcode::STORE,  0b001},
    {"SW",     &ExecSW,    Opcode::STORE,  0b010},
    {"ADDI",   &ExecADDI,  Opcode::OP_IMM, 0b000},
    {"SLTI",   &ExecSLTI,  Opcode::OP_IMM, 0b010},
    {"SLTIU",  &ExecSLTIU, Opcode::OP_IMM, 0b011},
    {"XORI",   &ExecXORI,  Opcode::OP_IMM, 0b100},
    {"ORI",    &ExecORI,   Opcode::OP_IMM, 0b110},
    {"ANDI",   &ExecANDI,  Opcode::OP_IMM, 0b111},
    {"SLLI",   &ExecSLLI,  Opcode::OP_IMM, 0b001},
    {"SRLI",   &ExecSRLI,  Opcode::OP_IMM, 0b101},
    {"SRAI",   &ExecSRAI,  Opcode::OP_IMM, 0b101},
    {"ADD",    &ExecADD,   Opcode::OP,     0b000, 0b0000000},
    {"SUB",    &ExecSUB,   Opcode::OP,     0b000, 0b0100000},
    {"SLL",    &ExecSLL,   Opcode::OP,     0b001, 0b0000000},
    {"SLT",    &ExecSLT,   Opcode::OP,     0b010, 0b0000000},
    {"SLTU",   &ExecSLTU,  Opcode::OP,     0b011, 0b0000000},
    {"XOR",    &ExecXOR,   Opcode::OP,     0b100, 0b0000000},
    {"SRL",    &ExecSRL,   Opcode::OP,     0b101, 0b0000000},
    {"SRA",    &ExecSRA,   Opcode::OP,     0b101, 0b0100000},
    {"OR",     &ExecOR,    Opcode::OP,     0b110, 0b0000000},
    {"AND",    &ExecAND,   Opcode::OP,     0b111, 0b0000000},
    {"ECALL",  &ExecECALL, Opcode::SYSTEM, 0b000},
    {"EBREAK", &ExecDummy, Opcode::SYSTEM, 0b000},
    {"URET",   &ExecDummy, Opcode::SYSTEM, 0b000},
    {"SRET",   &ExecDummy, Opcode::SYSTEM, 0b000},
    {"MRET",   &ExecDummy, Opcode::SYSTEM, 0b000},
    {"WFI",    &ExecDummy, Opcode::SYSTEM, 0b000},
    {"CSRRW",  &ExecDummy, Opcode::SYSTEM, 0b001},
    {"CSRRS",  &ExecDummy, Opcode::SYSTEM, 0b010},
    {"CSRRC",  &ExecDummy, Opcode::SYSTEM, 0b011},
    {"CSRRWI", &ExecDummy, Opcode::SYSTEM, 0b101},
    {"CSRRSI", &ExecDummy, Opcode::SYSTEM, 0b110},
    {"CSRRCI", &ExecDummy, Opcode::SYSTEM, 0b111},
    // RV32M standard extension
    {"MUL",    &ExecMUL,   Opcode::OP,     0b000, 0b0000001},
    {"MULH",   &ExecMULH,  Opcode::OP,     0b001, 0b0000001},
    {"MULHSU", &ExecMULHSU,Opcode::OP,     0b010, 0b0000001},
    {"MULHU",  &ExecMULHU, Opcode::OP,     0b011, 0b0000001},
    {"DIV",    &ExecDIV,   Opcode::OP,     0b100, 0b0000001},
    {"DIVU",   &ExecDIVU,  Opcode::OP,     0b101, 0b0000001},
    {"REM",    &ExecREM,   Opcode::OP,     0b110, 0b0000001},
    {"REMU",   &ExecREMU,  Opcode::OP,     0b111, 0b0000001}
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
}   // namespace isa
