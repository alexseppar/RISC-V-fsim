#include "decoder.h"

Inst Decoder::Decode(uint32_t pos) const
{
    assert(pos < m_buf.size());
    uint32_t inst = m_buf[pos];
    uint8_t opc = 0x7f & inst;

    switch (opc)
    {
        case 0b110011:
            Inst newInst;
            newInst.m_format = InstFormat::RT;
            newInst.m_rd = (inst >> 7) & 0x1f;
            newInst.m_rs1 = (inst >> 15) & 0x1f;
            newInst.m_rs2 = (inst >> 20) & 0x1f;
            uint8_t funct3 = (inst >> 12) & 0x7;
            uint8_t funct7 = (inst >> 25) & 0x7f;
            switch (funct3)
            {
                case 0b0:
                    newInst.m_name = funct7 == 0 ? ADD : 
                                    (funct7 == 32 ? SUB : MUL);
                    return newInst;

                case 0b010:
                    newInst.m_name = SLT;
                    return newInst;

                case 0b100:
                    newInst.m_name = XOR;
                    return newInst;

                case 0b101:
                    newInst.m_name = funct7 ? SRA : SRL;
                    return newInst;

                case 0b111:
                    newInst.m_name = AND;
                    return newInst;

                case 0b011:
                    newInst.m_name = SLTU;
                    return newInst;

                case 0b001:
                    newInst.m_name = SLL;
                    return newInst;

                case 0b110:
                    newInst.m_name = OR;
                    return newInst;
                
                default:
                    assert(!"fatal");
            }
            break;

        default:
            assert(!"Not supported");
    }
 }
