#include "single_core.hpp"

namespace svm
{
    Trap SingleCore::OR_rr(Regs first, Regs second) noexcept
    {
        std::pair<RegisterHalf, RegisterHalf> &valFirst = reg[first], &valSecond = reg[second];
        valFirst.first |= valSecond.first;
        valFirst.second |= valSecond.second;
        return Trap::OK;
    }

    Trap SingleCore::OR_mr(MemoryAddress addr, Regs second) noexcept
    {
        auto [trap, lvalue] = mem->read(addr);
        T rvalue = readRegister(second);
        if (trap == Trap::OK)
        {
            lvalue |= rvalue;
            trap = mem->write(addr, lvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }

    Trap SingleCore::OR_rm(Regs first, MemoryAddress addr) noexcept
    {
        auto regValue = readRegister(first);
        auto [trap, rvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            regValue |= rvalue;
            writeRegister(first, regValue);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    Trap SingleCore::OR_ri(Regs first, Immediate value) noexcept
    {
        auto regValue = readRegister(first);
        regValue |= value;
        writeRegister(first, regValue);
        return Trap::OK;
    }
    Trap SingleCore::OR_mi(MemoryAddress addr, Immediate value) noexcept
    {
        auto [trap, lvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            lvalue |= value;
            trap = mem->write(addr, lvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }
} // namespace svm
