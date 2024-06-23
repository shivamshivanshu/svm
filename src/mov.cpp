#include "single_core.hpp"
namespace svm
{
    Trap SingleCore::MOV_rr(Regs first, Regs second) noexcept
    {
        auto rvalue = readRegister(second);
        writeRegister(first, rvalue);
        return Trap::OK;
    }

    Trap SingleCore::MOV_rm(Regs first, MemoryAddress addr) noexcept
    {
        auto [trap, rvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            writeRegister(first, rvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }

    Trap SingleCore::MOV_ri(Regs first, Immediate value) noexcept
    {
        writeRegister(first, value);
        return Trap::OK;
    }

    Trap SingleCore::MOV_mr(MemoryAddress addr, Regs second) noexcept
    {
        auto rvalue = readRegister(second);
        auto trap = mem->write(addr, rvalue);
        return trap;
    }

    Trap SingleCore::MOV_mi(MemoryAddress addr, Immediate value) noexcept
    {
        auto trap = mem->write(addr, value);
        return trap;
    }
} // namespace svm
