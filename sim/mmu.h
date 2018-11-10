#ifndef MMU_H
#define MMU_H

#include "common.h"

class MMU
{
private:
    enum AccessType
    {
        READ,
        WRITE,
        EXEC
    };

    struct PhysAddr
    {
    private:
        MMU &mmu_;
        uint64_t pa_;

    public:
        PhysAddr(MMU &mmu, uint32_t va, AccessType type)
            : mmu_(mmu)
            , pa_(mmu_.Translate(va, type))
        {
        }

        inline uint64_t GetPA() const
        {
            return pa_;
        }
    };

    const uint32_t pagesize_ = 4096u;
    const uint32_t ppn2_ = 24;
    const uint8_t levels_ = 2u;
    const uint8_t ptesize_ = 4u;
    LRUCache<uint32_t, PhysAddr> instTLB_;
    LRUCache<uint32_t, PhysAddr> dataTLB_;
    uint8_t *pmem_;
    uint64_t pmem_size_;
    uint32_t &satp_;

public:
    MMU(uint8_t *pmem, uint64_t pmem_size, uint32_t &satp)
        : instTLB_(256)
        , dataTLB_(256)
        , pmem_(pmem)
        , pmem_size_(pmem_size)
        , satp_(satp)
    {
    }

    MMU(const MMU &that) = delete;
    MMU(MMU &&that) = delete;

    ~MMU() {}

    void Flush()
    {
        instTLB_.Clear();
        dataTLB_.Clear();
    }

    uint64_t Translate(uint32_t va, AccessType acc)
    {
        if (satp_ >> 31 == 0)
        {
            return va;
        }
        uint64_t table_pa = (satp_ & 0x3fffff) * pagesize_;
        int8_t i = levels_ - 1;
        uint32_t *pte;

        while (1)
        {
            pte = reinterpret_cast<uint32_t *>(pmem_ + table_pa +
                                               ptesize_ * (i ? va >> 22 : ((va << 10) >> 22)));

            if ((*pte & 1) == 0 || ((*pte & 2) == 0 && (*pte & 4) == 4))
            {
                throw SimException("page fault valid or R W");
            }

            if ((*pte & 2) != 0 || (*pte & 8) != 0)
            {
                break;
            }

            if (--i < 0)
            {
                throw SimException("page fault i < 0");
            }
            else
            {
                table_pa = (*pte >> 10) * pagesize_;
                continue;
            }
        }

        if ((acc == READ && (*pte & 2) == 0) || (acc == WRITE && (*pte & 4) == 0) ||
            (acc == EXEC && (*pte & 8) == 0))
        {
            throw SimException("page fault");
        }

        if (i > 0 && ((*pte >> 10) & 0x3ff) != 0)
        {
            throw SimException("page fault");
        }

        // Access bit
        *pte |= 64;

        if (acc == WRITE)
        {
            // Dirty bit
            *pte |= 128;
        }

        uint64_t pa = static_cast<uint64_t>(*pte) << 2;
        return (i ? (va & 0x003fffff) | (pa & (~1ull << 22))
                  : (va & 0xfff) | (pa & (~1ull << 12)));
    }

    uint32_t Load(uint32_t va, uint8_t nbytes, bool instRead = true)
    {
        uint64_t pa;

        if (satp_ >> 31 == 0)
        {
            pa = va;
        }
        else
        {
            auto res = instRead ? instTLB_.Insert(va, *this, va, MMU::AccessType::EXEC)
                                : dataTLB_.Insert(va, *this, va, MMU::AccessType::READ);
            pa = res.first.GetPA();
        }
        return (*reinterpret_cast<uint32_t *>(pmem_ + pa)) &
               (nbytes == 4 ? 0xffffffff : ((1 << (8 * nbytes)) - 1));
    }

    void Store(uint32_t va, uint8_t nbytes, uint32_t data)
    {
        uint64_t pa;

        if (satp_ >> 31 == 0)
        {
            pa = va;
        }
        else
        {
            auto res = dataTLB_.Insert(va, *this, va, MMU::AccessType::WRITE);
            pa = res.first.GetPA();
        }
        *reinterpret_cast<uint32_t *>(pmem_ + pa) =
            data & (nbytes == 4 ? 0xffffffff : ((1 << (8 * nbytes)) - 1));
    }
};

#endif
