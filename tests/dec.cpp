#include "../decoder.h"


int main()
{


    std::vector<uint32_t> buf = {0x007b01b3u, 0x407b01b3u, 
                                 0x007b71b3u, 0x007b61b3u,
                                 0x007b41b3u, 0x007b21b3u,
                                 0x007b31b3u, 0x407b51b3u,
                                 0x007b51b3u, 0x007b11b3u,
                                 0x027b01b3u};
    
    InstName ans[] = {ADD, SUB, AND, OR, 
                      XOR, SLT, SLTU, SRA,
                      SRL, SLL, MUL};

    Decoder dec(buf);

    for (int i = 0; i < buf.size(); ++i)
    {
        auto inst = dec.Decode(i);
        assert(inst.m_name = ans[i]);
        assert(inst.m_rd = 3);
        assert(inst.m_rs1 = 22);
        assert(inst.m_rs2 = 7);
    }
    return 0;
}
