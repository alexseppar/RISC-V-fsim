#ifndef DECODER_H
#define DECODER_H

#include "ir.h"
#include <array>
#include <vector>

namespace sim
{
class Decoder
{
private:
    std::array<isa::CmdFormat, 32> opc_to_fmt_;
    std::array<uint8_t, 256> opc_funct3_to_cmd_;
    std::vector<std::array<isa::Cmd, 128>> funct7_to_cmd_;

    isa::Cmd GetCmd(uint8_t opcode, uint8_t funct3 = 0) const;
    isa::Cmd GetCmd(uint8_t opcode, uint8_t funct3, uint8_t funct7) const;

public:
    Decoder();
    ir::Inst Decode(uint32_t cmd) const;
};
}   // namespace sim
#endif
