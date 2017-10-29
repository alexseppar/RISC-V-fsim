#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <cstdint>
#include <cassert>

enum class InstName {
    INVALID,
    ADD,
    SUB,
    OR,
    XOR,
    AND,
    SLT,
    SLTU,
    SRA,
    SRL,
    SLL,
    MUL,
    /*ADDI,
    SUBI,
    XORI,
    ANDI,
    SLTI,
    SLTIU,
    SRAI,
    SRLI,
    SLLI,
    LUI*/
};

enum class InstFormat {
    INVALID,
    RT,
    IT,
    ST,
    UT
};

struct Inst {

    InstName m_name = InstName::INVALID;
    InstFormat m_format = InstFormat::INVALID;

    uint8_t m_rd = 0,
            m_rs1 = 0,
            m_rs2 = 0;
    uint32_t m_imm = 0;
};


class Decoder {
    private:

    std::vector<uint32_t> m_buf; //Buffer with instructions to decode

    uint32_t m_pos; //Position in buffer
    
    public:

    explicit Decoder(const std::vector<uint32_t>& buf) : m_buf(buf), m_pos(0)
    {}

    explicit Decoder(std::vector<uint32_t>&& buf) : m_buf(buf), m_pos(0)
    {}

    Inst Decode(uint32_t pos) const;

};


#endif
