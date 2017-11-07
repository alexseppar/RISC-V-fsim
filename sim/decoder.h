#ifndef DECODER_H
#define DECODER_H

#include "ir.h"
#include <unordered_map>

namespace sim
{
class Decoder
{
private:
    std::unordered_map<uint8_t, isa::CmdFormat> op_fmt_;
    std::unordered_map<uint16_t, isa::Cmd> op_cmd_;

    isa::Cmd GetCmd(uint8_t opcode, uint8_t funct3 = 0, uint8_t funct7 = 0) const;

public:
    Decoder();
    ir::Inst Decode(uint32_t cmd) const;
};
}
#endif
