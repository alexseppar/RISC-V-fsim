#include "isa_desc.h"
#include "common.h"
#include <cassert>

namespace isa
{
// clang-format off
static constexpr OpcodeDesc opcode_desc[] =
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
    {0b01101, CmdFormat::U}, // LUI
    {0b00011, CmdFormat::I}  // MSCMEM
};

static constexpr CmdDesc cmd_desc[] =
{
    {"LUI",    &ExecLUI,   &TranslateUnsup, Opcode::LUI   },
    {"AUIPC",  &ExecAUIPC, &TranslateUnsup, Opcode::AUIPC },
    {"JAL",    &ExecJAL,   &TranslateUnsup, Opcode::JAL   },
    {"JALR",   &ExecJALR,  &TranslateUnsup, Opcode::JALR,   0b000},
    {"BEQ",    &ExecBEQ,   &TranslateUnsup, Opcode::BRANCH, 0b000},
    {"BNE",    &ExecBNE,   &TranslateUnsup, Opcode::BRANCH, 0b001},
    {"BLT",    &ExecBLT,   &TranslateUnsup, Opcode::BRANCH, 0b100},
    {"BGE",    &ExecBGE,   &TranslateUnsup, Opcode::BRANCH, 0b101},
    {"BLTU",   &ExecBLTU,  &TranslateUnsup, Opcode::BRANCH, 0b110},
    {"BGEU",   &ExecBGEU,  &TranslateUnsup, Opcode::BRANCH, 0b111},
    {"LB",     &ExecLB,    &TranslateUnsup, Opcode::LOAD,   0b000},
    {"LH",     &ExecLH,    &TranslateUnsup, Opcode::LOAD,   0b001},
    {"LW",     &ExecLW,    &TranslateUnsup, Opcode::LOAD,   0b010},
    {"LBU",    &ExecLBU,   &TranslateUnsup, Opcode::LOAD,   0b100},
    {"LHU",    &ExecLHU,   &TranslateUnsup, Opcode::LOAD,   0b101},
    {"SB",     &ExecSB,    &TranslateUnsup, Opcode::STORE,  0b000},
    {"SH",     &ExecSH,    &TranslateUnsup, Opcode::STORE,  0b001},
    {"SW",     &ExecSW,    &TranslateUnsup, Opcode::STORE,  0b010},
    {"ADDI",   &ExecADDI,  &TranslateUnsup, Opcode::OP_IMM, 0b000},
    {"SLTI",   &ExecSLTI,  &TranslateUnsup, Opcode::OP_IMM, 0b010},
    {"SLTIU",  &ExecSLTIU, &TranslateUnsup, Opcode::OP_IMM, 0b011},
    {"XORI",   &ExecXORI,  &TranslateUnsup, Opcode::OP_IMM, 0b100},
    {"ORI",    &ExecORI,   &TranslateUnsup, Opcode::OP_IMM, 0b110},
    {"ANDI",   &ExecANDI,  &TranslateUnsup, Opcode::OP_IMM, 0b111},
    {"SLLI",   &ExecSLLI,  &TranslateUnsup, Opcode::OP_IMM, 0b001},
    {"SRLI",   &ExecSRLI,  &TranslateUnsup, Opcode::OP_IMM, 0b101},
    {"SRAI",   &ExecSRAI,  &TranslateUnsup, Opcode::OP_IMM, 0b101},
    {"ADD",    &ExecADD,   &TranslateUnsup, Opcode::OP,     0b000, 0b0000000},
    {"SUB",    &ExecSUB,   &TranslateUnsup, Opcode::OP,     0b000, 0b0100000},
    {"SLL",    &ExecSLL,   &TranslateUnsup, Opcode::OP,     0b001, 0b0000000},
    {"SLT",    &ExecSLT,   &TranslateUnsup, Opcode::OP,     0b010, 0b0000000},
    {"SLTU",   &ExecSLTU,  &TranslateUnsup, Opcode::OP,     0b011, 0b0000000},
    {"XOR",    &ExecXOR,   &TranslateUnsup, Opcode::OP,     0b100, 0b0000000},
    {"SRL",    &ExecSRL,   &TranslateUnsup, Opcode::OP,     0b101, 0b0000000},
    {"SRA",    &ExecSRA,   &TranslateUnsup, Opcode::OP,     0b101, 0b0100000},
    {"OR",     &ExecOR,    &TranslateUnsup, Opcode::OP,     0b110, 0b0000000},
    {"AND",    &ExecAND,   &TranslateUnsup, Opcode::OP,     0b111, 0b0000000},
    {"ECALL",  &ExecECALL, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"EBREAK", &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"URET",   &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"SRET",   &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"MRET",   &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"WFI",    &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b000},
    {"FENCE",  &ExecFENCE, &TranslateUnsup, Opcode::MSCMEM, 0b000},
    {"CSRRW",  &ExecCSRRW, &TranslateUnsup, Opcode::SYSTEM, 0b001},
    {"CSRRS",  &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b010},
    {"CSRRC",  &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b011},
    {"CSRRWI", &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b101},
    {"CSRRSI", &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b110},
    {"CSRRCI", &ExecDummy, &TranslateUnsup, Opcode::SYSTEM, 0b111},
    // RV32M standard extension
    {"MUL",    &ExecMUL,   &TranslateUnsup, Opcode::OP,     0b000, 0b0000001},
    {"MULH",   &ExecMULH,  &TranslateUnsup, Opcode::OP,     0b001, 0b0000001},
    {"MULHSU", &ExecMULHSU,&TranslateUnsup, Opcode::OP,     0b010, 0b0000001},
    {"MULHU",  &ExecMULHU, &TranslateUnsup, Opcode::OP,     0b011, 0b0000001},
    {"DIV",    &ExecDIV,   &TranslateUnsup, Opcode::OP,     0b100, 0b0000001},
    {"DIVU",   &ExecDIVU,  &TranslateUnsup, Opcode::OP,     0b101, 0b0000001},
    {"REM",    &ExecREM,   &TranslateUnsup, Opcode::OP,     0b110, 0b0000001},
    {"REMU",   &ExecREMU,  &TranslateUnsup, Opcode::OP,     0b111, 0b0000001}
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
