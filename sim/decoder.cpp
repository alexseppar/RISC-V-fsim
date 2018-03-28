#include "decoder.h"
#include "common.h"
#include <cstdio>
#include <cstring>

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
    fprintf(options::log, "Decoder initialization started\n");
    // setup opc_to_fmt_ table
    opc_to_fmt_.fill(isa::CmdFormat::UNDEFINED);
    for (size_t i = 0; i < isa::GetOpcodesNum(); ++i)
    {
        const isa::OpcodeDesc &op_desc = isa::GetOpcodeDesc(i);
        assert(op_desc.opcode < 32);
        if (opc_to_fmt_[op_desc.opcode] != isa::CmdFormat::UNDEFINED)
            fprintf(options::log, "Warning: ISA opcode description has same opcodes\n");
        opc_to_fmt_[op_desc.opcode] = op_desc.format;
    }
    // setup opc_funct3_[funct7_]to_cmd_ tables
    opc_funct3_to_cmd_.fill(static_cast<uint8_t>(isa::Cmd::UNDEFINED));
    for (size_t i = 0; i < isa::GetCmdsNum(); ++i)
    {
        const isa::CmdDesc &cmd_desc = isa::GetCmdDesc(i);
        uint8_t opcode = isa::GetOpcodeDesc(cmd_desc.opcode).opcode;
        assert(opcode < 32);
        uint16_t opc_funct3 = (cmd_desc.funct3 << 5) | opcode;
        isa::CmdFormat format = opc_to_fmt_[opcode];
        assert(format != isa::CmdFormat::UNDEFINED);
        if (format == isa::CmdFormat::R)
        {
            // this format uses funct7
            if (opc_funct3_to_cmd_[opc_funct3] == static_cast<uint8_t>(isa::Cmd::UNDEFINED))
            {
                // first use of opc_funct3
                opc_funct3_to_cmd_[opc_funct3] = funct7_to_cmd_.size();
                std::array<isa::Cmd, 128> funct7;
                funct7.fill(isa::Cmd::UNDEFINED);
                funct7_to_cmd_.push_back(std::move(funct7));
            }
            if (funct7_to_cmd_[opc_funct3_to_cmd_[opc_funct3]][cmd_desc.funct7] !=
                isa::Cmd::UNDEFINED)
                fprintf(options::log, "Warning: cmd %s has opcode, funct3, funct7 collision\n",
                        cmd_desc.name);
            funct7_to_cmd_[opc_funct3_to_cmd_[opc_funct3]][cmd_desc.funct7] =
                static_cast<isa::Cmd>(i);
        }
        else
        {
            if (opc_funct3_to_cmd_[opc_funct3] != static_cast<uint8_t>(isa::Cmd::UNDEFINED))
                fprintf(options::log, "Warning: cmd %s has opcode, funct3 collision\n",
                        cmd_desc.name);
            opc_funct3_to_cmd_[opc_funct3] = i;
        }
    }
    funct7_to_cmd_.shrink_to_fit();
    fprintf(options::log, "Decoder initialization finished\n");
}

isa::Cmd Decoder::GetCmd(uint8_t opcode, uint8_t funct3) const
{
    uint8_t op_funct3 = (funct3 << 5) | opcode;
    isa::Cmd cmd = static_cast<isa::Cmd>(opc_funct3_to_cmd_[op_funct3]);
    if (cmd == isa::Cmd::UNDEFINED)
    {
        throw SimException("Can not find instruction by opcode, funct3");
    }
    else
    {
        return cmd;
    }
}

isa::Cmd Decoder::GetCmd(uint8_t opcode, uint8_t funct3, uint8_t funct7) const
{
    uint8_t op_funct3 = (funct3 << 5) | opcode;
    isa::Cmd cmd = funct7_to_cmd_[opc_funct3_to_cmd_[op_funct3]][funct7];
    if (cmd == isa::Cmd::UNDEFINED)
    {
        throw SimException("Can not find instruction by opcode, funct3, funct7");
    }
    else
    {
        return cmd;
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
    isa::CmdFormat format = opc_to_fmt_[opcode];
    if (format == isa::CmdFormat::UNDEFINED)
    {
        throw SimException("Opcode is not supported");
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
        uint32_t imm = fmt.imm;
        // process some special cases
        // shift instructions
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
        // ECALL, EBREAK, URET, MRET, SRET, WFI instructions
        else if ((uint8_t)cmd >= (uint8_t)isa::Cmd::ECALL &&
                 (uint8_t)cmd <= (uint8_t)isa::Cmd::WFI)
        {
            if (fmt.rs1 || fmt.rd)
            {
                throw SimException("SYSTEM instruction: rs1, rd are not zero");
            }
            switch (imm)
            {
            case 0x000:
                cmd = isa::Cmd::ECALL;
                break;
            case 0x001:
                cmd = isa::Cmd::EBREAK;
                break;
            case 0x002:
                cmd = isa::Cmd::URET;
                break;
            case 0x102:
                cmd = isa::Cmd::SRET;
                break;
            case 0x302:
                cmd = isa::Cmd::MRET;
                break;
            case 0x105:
                cmd = isa::Cmd::WFI;
                break;
            default:
                throw SimException("SYSTEM instruction: incorrect immediate");
            }
        }
        // other instructions
        else
        {
            imm = SignExtend(fmt.imm, 11);
        }
        return ir::GenInst<isa::CmdFormat::I>(cmd, fmt.rd, fmt.rs1, imm);
    }
    // S & B format
    else if (format == isa::CmdFormat::S || format == isa::CmdFormat::B)
    {
        isa::SFormat fmt;
        memcpy((void *)&fmt, (void *)&command, 4);
        isa::Cmd cmd = GetCmd(opcode, fmt.funct3);
        if (format == isa::CmdFormat::S)
        {
            uint32_t imm = SignExtend((uint32_t)fmt.imm2 << 5 | fmt.imm1, 11);
            return ir::GenInst<isa::CmdFormat::S>(cmd, imm, fmt.rs1, fmt.rs2);
        }
        if (format == isa::CmdFormat::B)
        {
            uint32_t imm = SignExtend((uint32_t)fmt.imm1 >> 1 | (fmt.imm2 & 0x3f) << 4 |
                                          (fmt.imm1 & 1) << 10 | (fmt.imm2 >> 6) << 11,
                                      11);
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
            return ir::GenInst<isa::CmdFormat::U>(cmd, fmt.rd, fmt.imm << 12);
        }
        if (format == isa::CmdFormat::J)
        {
            uint32_t imm = SignExtend((fmt.imm >> 9 & 0x3ff) | (fmt.imm >> 8 & 1) << 10 |
                                          (fmt.imm & 0xff) << 11 | (fmt.imm >> 19) << 19,
                                      19);
            return ir::GenInst<isa::CmdFormat::J>(cmd, fmt.rd, imm);
        }
    }
    throw SimException("Decoding of this format is not implemented");
    // should not execute, need to suppress g++ warning
    return ir::Inst(isa::Cmd::UNDEFINED, 0, 0, 0, 0);
}
}   // namespace sim
