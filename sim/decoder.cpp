#include "decoder.h"
#include "common.h"
#include <cstring>
#include <iomanip>
#include <iostream>

namespace sim
{
uint32_t SignExtend(uint32_t imm, uint8_t n)
{
    assert(n < 32);
    uint32_t mask = (~0u) << n;
    return (imm & (1u << n)) ? (imm | mask) : imm;
}

Decoder::Decoder()
{
    std::cerr << "Decoder initialization started" << std::endl;
    // setup op_fmt_ hash table
    for (size_t i = 0; i < isa::GetOpcodesNum(); ++i)
    {
        const isa::OpcodeDesc &op_desc = isa::GetOpcodeDesc(i);
        bool res = op_fmt_.insert({op_desc.opcode, op_desc.format}).second;
        if (!res)
        {
            std::cerr << "Warning: ISA description has same opcodes: "
                      << op_desc.opcode << std::endl;
        }
    }
    // setup opcode_cmd_ hash table
    for (size_t i = 0; i < isa::GetCmdsNum(); ++i)
    {
        const isa::CmdDesc &cmd_desc = isa::GetCmdDesc(i);
        uint16_t cmd = (cmd_desc.funct7 << 8) | (cmd_desc.funct3 << 5) |
                       isa::GetOpcodeDesc(cmd_desc.opcode).opcode;
        auto res = op_cmd_.insert({cmd, static_cast<isa::Cmd>(i)});
        if (!res.second)
        {
            std::cerr << "Warning: ISA description has same instructions: "
                      << isa::GetCmdDesc(res.first->second).name << " and "
                      << cmd_desc.name << std::endl;
        }
    }
    std::cerr << "Decoder initialization finished" << std::endl;
}

isa::Cmd Decoder::GetCmd(uint8_t opcode, uint8_t funct3, uint8_t funct7) const
{
    uint16_t cmd = (funct7 << 8) | (funct3 << 5) | (opcode);
    auto cmd_it = op_cmd_.find(cmd);
    if (cmd_it == op_cmd_.end())
    {
        throw SimException("Can not find instruction by opcode, funct3, funct7");
    }
    else
    {
        return cmd_it->second;
    }
}

ir::Inst Decoder::Decode(uint32_t command) const
{
    // step 1: check first 2 bits
    uint8_t sz = command & 0b11;
    if (sz != 0b11)
    {
        throw SimException("Packed instructions are not supported!");
    }
    // step 2: get opcode and instruction format
    uint8_t opcode = (command & ((1u << 7) - 1)) >> 2;
    isa::CmdFormat format;
    auto op_fmt_it = op_fmt_.find(opcode);
    if (op_fmt_it == op_fmt_.end())
    {
        throw SimException("Opcode is not supported");
    }
    else
    {
        format = op_fmt_it->second;
    }
    // step 3: use instruction format to decode instruction
    // R format
    if (format == isa::CmdFormat::R)
    {
        isa::RFormat fmt;
        memcpy((void *)&fmt, (void *)&command, 4);
        isa::Cmd cmd = GetCmd(opcode, fmt.funct3, fmt.funct7);
        return ir::GenInst<isa::CmdFormat::R>(cmd, fmt.rd, fmt.rs1, fmt.rs2);
    }
    // I format
    else if (format == isa::CmdFormat::I)
    {
        isa::IFormat fmt;
        memcpy((void *)&fmt, (void *)&command, 4);
        isa::Cmd cmd = GetCmd(opcode, fmt.funct3);
        // need to do some checks for shift instructions
        uint32_t imm = SignExtend(fmt.imm, 11);
        if (cmd == isa::Cmd::SLLI)
        {
            if ((imm >> 5) != 0u)
            {
                throw SimException("SLLI: incorrect immediate");
            }
        }
        else if (cmd == isa::Cmd::SRLI || cmd == isa::Cmd::SRAI)
        {
            uint32_t funct7 = imm >> 5;
            if (funct7 == 0b0000000)
            {
                cmd = isa::Cmd::SRLI;
            }
            else if (funct7 == 0b0100000)
            {
                cmd = isa::Cmd::SRAI;
                imm = imm & ((1u << 6) - 1);
            }
            else
            {
                throw SimException("SRLI/SRAI: incorrect immediate");
            }
        }
        return ir::GenInst<isa::CmdFormat::I>(cmd, fmt.rd, fmt.rs1, imm);
    }
    // S & B format
    else if (format == isa::CmdFormat::S || format == isa::CmdFormat::B)
    {
        isa::SFormat fmt;
        memcpy((void *)&fmt, (void *)&command, 4);
        isa::Cmd cmd = GetCmd(opcode, fmt.funct3);
        uint32_t imm = SignExtend(((uint32_t)fmt.imm2 << 5) | fmt.imm1, 11);
        if (format == isa::CmdFormat::S)
        {
            return ir::GenInst<isa::CmdFormat::S>(cmd, imm, fmt.rs1, fmt.rs2);
        }
        if (format == isa::CmdFormat::B)
        {
            return ir::GenInst<isa::CmdFormat::B>(cmd, imm, fmt.rs1, fmt.rs2);
        }
    }
    // U & J format
    else if (format == isa::CmdFormat::U || format == isa::CmdFormat::J)
    {
        isa::UFormat fmt;
        memcpy((void *)&fmt, (void *)&command, 4);
        isa::Cmd cmd = GetCmd(opcode);
        if (format == isa::CmdFormat::U)
        {
            return ir::GenInst<isa::CmdFormat::U>(cmd, fmt.rd, fmt.imm);
        }
        if (format == isa::CmdFormat::J)
        {
            uint32_t imm = (int)(SignExtend(fmt.imm, 19)) * 2;
            return ir::GenInst<isa::CmdFormat::J>(cmd, fmt.rd, imm);
        }
    }
    else
    {
        throw SimException("Decoding of this format is not implemented");
    }
    // should not execute, need to suppress g++ warning
    return ir::Inst(static_cast<isa::Cmd>(0), 0, 0, 0, 0);
}
}
