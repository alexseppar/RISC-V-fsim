#include "sim.h"

FILE *log = stderr;

int main(int argc, char *argv[])
{
    try
    {
        // TODO: as minimum, process args (logging, simulator modes, etc)
        std::vector<uint32_t> cmds = {
            0b00000000001100111000000010110011,   // ADD x7,x3->x1
            0b11001010010100101000010100010011    // ADDI x5,CA5->x10
        };
        sim::Sim simulator(cmds);
        simulator.Execute();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
