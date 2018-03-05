#include "elf_reader.h"
#include "options.h"
#include "sim.h"

int main(int argc, char *argv[])
{
    try
    {
        options::ParseOptions(argc, argv);
        options::OpenLog();

        elf::Elf_reader er;
        er.Init(options::elf_file);
        std::vector<uint32_t> cmds;
        uint32_t pc = 0;
        if (!er.Load(cmds, pc))
            fprintf(options::log, "Can't load segment\n");

        sim::Sim simulator(cmds, pc);
        simulator.Execute();
        options::CloseLog();
    }
    catch (std::exception &e)
    {
        printf("%s\n", e.what());
    }
    return 0;
}
