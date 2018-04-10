#include "elf_reader.h"

namespace elf
{

template<typename... Args>
static inline void print_and_exit(const char *msg, const Args &... args)
{
    fprintf(options::log, msg, args...);
    exit(0);
}

void Elf_reader::Init(const char *filename)
{
    if (e_ || fd_)
        Clear();

    if (elf_version(EV_CURRENT) == EV_NONE)
        print_and_exit("Elf: library initialization failed: %s\n", elf_errmsg(-1));

    fd_ = open(filename, O_RDONLY, 0);
    if (fd_ < 0)
        print_and_exit("Elf: can't open the object file %s\n", filename);

    e_ = elf_begin(fd_, ELF_C_READ, nullptr);
    if (!e_)
        print_and_exit("Elf: elf_begin() failed: %s\n", elf_errmsg(-1));

    if (elf_kind(e_) != ELF_K_ELF)
        print_and_exit("Elf: Elf_Kind isn't an object\n");

    GElf_Ehdr ehdr;
    if (!gelf_getehdr(e_, &ehdr))
        print_and_exit("Elf: gelf_getehdr() failed %s\n", elf_errmsg(-1));

    if (ehdr.e_machine != EM_RISCV)
        print_and_exit("Elf: %s is not a RISCV machine\n", filename);

    if (ehdr.e_ident[EI_CLASS] != ELFCLASS32)
        print_and_exit("Elf: %s is not a 32 bit machine\n", filename);

    if (elf_getphdrnum(e_, &phdrnum_))
        print_and_exit("Elf: elf_getphdrnum() failed %s\n", elf_errmsg(-1));

    entry_ = ehdr.e_entry;
}

bool Elf_reader::Load(std::vector<uint32_t> &cmds, uint32_t &va, uint32_t &offset)
{
    for (; cur_phdr_ < phdrnum_; ++cur_phdr_)
    {
        GElf_Phdr *gres = gelf_getphdr(e_, cur_phdr_, &phdr_);
        if (gres != &phdr_)
            print_and_exit("Elf: gelf_getphdr() failed %s\n", elf_errmsg(-1));

        if (phdr_.p_type != PT_LOAD)
            continue;

        long int off = phdr_.p_offset;
        if (off != lseek(fd_, off, SEEK_SET))
            print_and_exit("Elf: lseek failed\n");

        std::vector<uint32_t> buf(phdr_.p_memsz);
        int read_sz = read(fd_, &(buf[0]), phdr_.p_filesz);
        if (read_sz < 0 || phdr_.p_filesz != static_cast<uint32_t>(read_sz))
            print_and_exit("Elf: segment reading failed\n");
        ++cur_phdr_;
        std::swap(cmds, buf);
        va = phdr_.p_vaddr;
        offset = entry_;   //- va;   // phdr_.p_vaddr;
        return true;
    }
    return false;
}

void Elf_reader::Clear()
{
    elf_end(e_);
    e_ = nullptr;
    close(fd_);
    fd_ = 0;
}

Elf_reader::~Elf_reader() {}
}   // namespace elf
