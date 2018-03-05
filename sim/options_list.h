// clang-format off
//         variable    type      default  option name    arg name     description
OPTION_DEF(verbose,    bool,     false,   "--verbose",   nullptr,     "enable logging of instructions execution")
OPTION_DEF(log_file,   char *,   nullptr, "--log-file",  "filename",  "write excution output to file")
OPTION_DEF(cache_size, size_t,   256,     "--cache-size","size",      "number of saved traces in LRU cache")
OPTION_DEF(max_insts,  uint64_t, 1000000, "--max-insts", "insts num", "number of instructions, after which simulator stops")
OPTION_DEF(elf_file,   char *,   nullptr, "--elf",       "filename",  "elf file to execute")
// clang-format on
