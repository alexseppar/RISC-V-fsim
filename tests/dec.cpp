#include "../decoder.h"


int main()
{


    std::vector<uint32_t> buf = {0x007b01b3u, 0x407b01b3u, 
                                 0x007b71b3u, 0x007b61b3u,
                                 0x007b41b3u, 0x007b21b3u,
                                 0x007b31b3u, 0x407b51b3u,
                                 0x007b51b3u, 0x007b11b3u,
                                 0x027b01b3u};
    
    InstName ans[] = {InstName::ADD, InstName::SUB, InstName::AND, InstName::OR, 
                      InstName::XOR, InstName::SLT, InstName::SLTU, InstName::SRA,
                      InstName::SRL, InstName::SLL, InstName::MUL};

    Decoder dec(buf);

    for (int i = 0; i < buf.size(); ++i)
    {
        auto inst = dec.Decode(i);
        assert(inst->GetName() == ans[i]);
        assert(inst->GetOperand(0).GetRegNum() == 3);
        assert(inst->GetOperand(1).GetRegNum() == 22);
        assert(inst->GetOperand(2).GetRegNum() == 7);
        delete inst;
    }
    return 0;
}
