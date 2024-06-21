#include "memory.hpp"

namespace svm
{
    std::pair<Trap, T> Memory::read(MemoryAddress addr) const noexcept
    {
        Trap trap = Trap::SEG_FAULT;
        T value{0};
        if (addr <= memory.size())
        {
            trap = Trap::OK;
            value = memory[addr];
        }
        return {trap, value};
    }

    Trap Memory::write(MemoryAddress addr, Immediate value) noexcept 
    {
        Trap trap = Trap::SEG_FAULT;
        if(addr <= memory.size())
        {
            memory[addr] = value;
            trap = Trap::OK;
        }
        return trap;
    }
} // namespace svm
