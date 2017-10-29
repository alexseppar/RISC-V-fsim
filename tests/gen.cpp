#include "../inst.h"

int main()
{
    InstName names[] = {InstName::ADD, InstName::SUB, InstName::AND, InstName::OR, 
                      InstName::XOR, InstName::SLT, InstName::SLTU, InstName::SRA,
                      InstName::SRL, InstName::SLL, InstName::MUL};

    uint8_t rd = 1,
            rs1 = 2,
            rs2 = 3;

    for (int i = 0; i < 11; ++i)
    {
        Inst* inst = GenInst(names[i], InstFormat::RT).R(rd).R(rs1).R(rs2);
        assert(inst->GetName() == names[i]);
        assert(inst->GetOperand(0).GetRegNum() == rd);
        assert(inst->GetOperand(1).GetRegNum() == rs1);
        assert(inst->GetOperand(2).GetRegNum() == rs2);
        delete inst;
    }

    return 0;
}
