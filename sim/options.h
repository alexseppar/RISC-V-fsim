#ifndef OPTIONS_H
#define OPTIONS_H
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace options
{
extern bool verbose;
extern FILE *log;
extern size_t cache_size;
extern uint64_t max_insts;

void ParseOptions(int argc, char *argv[]);
void CloseFiles();
}
#endif
