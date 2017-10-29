#include "decoder.h" 

Inst* Decoder::Decode(uint32_t pos) const
{
    assert(pos < m_buf.size());
    uint32_t inst = m_buf[pos];
    uint8_t opc = 0x7f & inst;

    switch (opc)
    {
        case 0b110011:
            InstFormat format = InstFormat::RT;
            InstName name;
            uint8_t rd = (inst >> 7) & 0x1f;
            uint8_t rs1 = (inst >> 15) & 0x1f;
            uint8_t rs2 = (inst >> 20) & 0x1f;
            uint8_t funct3 = (inst >> 12) & 0x7;
            uint8_t funct7 = (inst >> 25) & 0x7f;
            switch (funct3)
            {
                case 0b0:
                    name = funct7 == 0 ? InstName::ADD : 
                          (funct7 == 32 ? InstName::SUB : InstName::MUL);
                    break;

                case 0b010:
                    name = InstName::SLT;
                    break;

                case 0b100:
                    name = InstName::XOR;
                    break;

                case 0b101:
                    name = funct7 ? InstName::SRA : InstName::SRL;
                    break;

                case 0b111:
                    name = InstName::AND;
                    break;

                case 0b011:
                    name = InstName::SLTU;
                    break;

                case 0b001:
                    name = InstName::SLL;
                    break;

                case 0b110:
                    name = InstName::OR;
                    break;
                
                default:
                    assert(!"fatal");
            }
            return GenInst(name, format).R(rd).R(rs1).R(rs2);

        default:
            assert(!"Not supported");
    }
 }
