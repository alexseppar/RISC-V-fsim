#include "options.h"
#include "sim.h"

int main(int argc, char *argv[])
{
    try
    {
        options::ParseOptions(argc, argv);
        std::vector<uint32_t> cmds = {
            0b00000000001100111000000010110011,   // ADD x7,x3->x1
            0b11001010010100101000010100010011,   // ADDI x5,CA5->x10
            0b11111111111111111100000001101111    // JAL [-4 * 2]
        };
        sim::Sim simulator(cmds);
        simulator.Execute();
        options::CloseFiles();
    }
    catch (std::exception &e)
    {
        printf("%s\n", e.what());
    }
    return 0;
}
