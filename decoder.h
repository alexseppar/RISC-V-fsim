#ifndef DECODER_H
#define DECODER_H

#include "inst.h"

class Decoder
{
private:
    std::vector<uint32_t> m_buf;   // Buffer with instructions to decode

    uint32_t m_pos;   // Position in buffer

public:
    explicit Decoder(const std::vector<uint32_t> &buf) : m_buf(buf), m_pos(0)
    {
    }

    explicit Decoder(std::vector<uint32_t> &&buf) : m_buf(buf), m_pos(0)
    {
    }

    Inst *
    Decode(uint32_t pos) const;
};

#endif
