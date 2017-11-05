#include "decoder.h"
#include "common.h"
#include <cstring>
#include <iostream>

namespace sim
{
Decoder::Decoder()
{
    std::cerr << "Decoder initialization started" << std::endl;
    // setup opcode_format_ hash table
    for (size_t i = 0; i < ArrSize(isa::opcode_desc); ++i)
    {
        auto res = opcode_format_.insert(
            {isa::opcode_desc[i].opcode, isa::opcode_desc[i].format});
        if (!res.second)
        {
            std::cerr << "Warning: opcode_desc has same opcodes "
                      << isa::opcode_desc[i].opcode << std::endl;
        }
    }
    // setup opcode_cmd_ hash table
    for (size_t i = 0; i < ArrSize(isa::cmd_desc); ++i)
    {
        uint16_t packed_opcode =
            (isa::cmd_desc[i].funct7 << 8) | (isa::cmd_desc[i].funct3 << 5) |
            isa::opcode_desc[static_cast<uint8_t>(isa::cmd_desc[i].opcode)].opcode;
        auto res = opcode_cmd_.insert({packed_opcode, static_cast<isa::Cmd>(i)});
        if (!res.second)
        {
            std::cerr
                << "Warning: cmd_desc has the same attributes for instructions "
                << isa::cmd_desc[static_cast<uint8_t>(res.first->second)].name
                << " and " << isa::cmd_desc[i].name << std::endl;
        }
    }
    std::cerr << "Decoder initialization finished" << std::endl;
}

isa::Cmd Decoder::GetCmd(uint8_t opcode, uint8_t funct3, uint8_t funct7) const
{
    uint16_t packed_opcode = (funct7 << 8) | (funct3 << 5) | (opcode);
    auto     op_cmd_it     = opcode_cmd_.find(packed_opcode);
    if (op_cmd_it == opcode_cmd_.end())
    {
        throw SimException("Can not recognize instruction");
    }
    else
    {
        return op_cmd_it->second;
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
    uint8_t        opcode = (command & ((1u << 7) - 1)) >> 2;
    isa::CmdFormat format;
    auto           op_fmt_it = opcode_format_.find(opcode);
    if (op_fmt_it == opcode_format_.end())
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
        uint32_t imm = fmt.imm;
        if (cmd == isa::Cmd::SLLI)
        {
            if ((imm >> 5) != 0u)
            {
                throw SimException("SLLI should contain leading zeroes");
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
                throw SimException("Can not select between SRLI and SRAI");
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
        uint32_t imm = ((uint32_t)fmt.imm2 << 5) | fmt.imm1;
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
            return ir::GenInst<isa::CmdFormat::J>(cmd, fmt.rd, fmt.imm);
        }
    }
    else
    {
        throw SimException("Unsupported format");
    }
}
}
