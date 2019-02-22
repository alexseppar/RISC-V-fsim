#ifndef MMU_H
#define MMU_H

#include "common.h"
#include <memory>

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
        uint64_t pa_;

    public:
        PhysAddr(const MMU &mmu, uint32_t va, AccessType type)
            : pa_(mmu.Translate(va, type))
        {
        }

        operator uint64_t() const
        {
            return pa_;
        }
    };

    mutable LRUCache<uint32_t, PhysAddr> instTLB_;
    mutable LRUCache<uint32_t, PhysAddr> dataTLB_;
    uint64_t pmem_size_;
    std::unique_ptr<uint8_t[]> pmem_;
    uint32_t &satp_;

public:
    static constexpr uint32_t pagesize = 4096u;
    static constexpr uint32_t ppn2 = 24;
    static constexpr uint8_t levels = 2;
    static constexpr uint8_t ptesize = 4;

    MMU(uint32_t &satp)
        : instTLB_(options::itlb_size)
        , dataTLB_(options::dtlb_size)
        , pmem_size_(options::mem_pages * MMU::pagesize)
        , pmem_(new uint8_t[pmem_size_])
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

    uint64_t GetMemSize() const
    {
        return pmem_size_;
    }

    template<typename T>
    T *GetMemPtr(uint32_t pa) const
    {
        if (pa + sizeof(T) >= pmem_size_)
            throw SimException("Address is out of memory");
        return reinterpret_cast<T *>(pmem_.get() + pa);
    }

    uint64_t Translate(uint32_t va, AccessType acc) const
    {
        if (satp_ >> 31 == 0)
        {
            return va;
        }
        uint64_t table_pa = (satp_ & 0x3fffff) * pagesize;
        int8_t i = levels - 1;
        uint32_t *pte;

        while (1)
        {
            pte = GetMemPtr<uint32_t>(table_pa + ptesize * (i ? va >> 22 : ((va << 10) >> 22)));

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
                table_pa = (*pte >> 10) * pagesize;
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
        return (i ? (va & 0x003fffff) | (pa & (~1ull << 22)) : (va & 0xfff) | (pa & (~1ull << 12)));
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
            pa = res.first;
        }
        return (*GetMemPtr<uint32_t>(pa)) & (nbytes == 4 ? 0xffffffff : ((1 << (8 * nbytes)) - 1));
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
            pa = res.first;
        }
        *GetMemPtr<uint32_t>(pa) = data & (nbytes == 4 ? 0xffffffff : ((1 << (8 * nbytes)) - 1));
    }
};

#endif
