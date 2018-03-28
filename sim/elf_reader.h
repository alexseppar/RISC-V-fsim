#ifndef ELF_READER_H
#define ELF_READER_H

#include "common.h"
#include <fcntl.h>
#include <gelf.h>
#include <string>
#include <unistd.h>
#include <vector>

namespace elf
{

class Elf_reader
{
private:
    Elf *e_ = nullptr;
    int fd_ = -1;
    GElf_Phdr phdr_;
    size_t phdrnum_ = 0;
    size_t cur_phdr_ = 0;
    uint64_t entry_ = 0;

public:
    Elf_reader() {}
    Elf_reader(const Elf_reader &) = delete;
    Elf_reader(Elf_reader &&) = delete;
    void Init(const char *filename);
    void Init(const std::string &filename)
    {
        return Init(filename.c_str());
    }
    void Clear();
    bool Load(std::vector<uint32_t> &cmds /* out */, uint32_t &offset /* out */);
    Elf_reader &operator=(const Elf_reader &) = delete;
    ~Elf_reader();
};
}   // namespace elf

#endif
