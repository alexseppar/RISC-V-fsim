#include "options.h"
#include "common.h"
#include <stdexcept>
#include <string>

namespace options
{
bool verbose = false;
FILE *log = stderr;
size_t cache_size = 256;
uint64_t max_insts = 1000000;

void ParseOptions(int argc, char *argv[])
{
    int i = 1;
    while (i < argc)
    {
        std::string arg = argv[i++];
        if (arg == "--verbose")
        {
            verbose = true;
        }
        else if (arg == "--log-file")
        {
            std::string option = argv[i++];
            log = fopen(option.c_str(), "w");
            if (!log)
                throw std::invalid_argument(std::string("Can not open file: ") + option);
        }
        else if (arg == "--cache-size")
        {
            std::string option = argv[i++];
            try
            {
                cache_size = std::stoull(option);
            }
            catch (std::exception &e)
            {
                printf("Unrecognized --cache-size option: %s\n", option.c_str());
                throw;
            }
        }
        else if (arg == "--max-insts")
        {
            std::string option = argv[i++];
            try
            {
                max_insts = std::stoull(option);
            }
            catch (std::exception &e)
            {
                printf("Unrecognized --max-insts option: %s\n", option.c_str());
                throw;
            }
        }
        else
        {
            throw std::invalid_argument(std::string("Unrecognized argument: ") + arg);
        }
    };
}

void CloseFiles()
{
    if (log)
    {
        fclose(log);
    }
}
}
