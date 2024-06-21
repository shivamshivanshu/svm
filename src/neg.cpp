#include "single_core.hpp"

namespace svm
{
    Trap SingleCore::NEG_r(Regs first)
    {
        auto lvalue = readRegister(first);
        lvalue = ~lvalue;
        writeRegister(first, lvalue);
        return Trap::OK;
    }

    Trap SingleCore::NEG_m(MemoryAddress addr)
    {
        auto [trap, lvalue] = mem->read(addr);
        if(trap == Trap::OK)
        {
            lvalue = ~lvalue;
            trap = mem->write(addr, lvalue);
            return trap;
        }
        else 
        {
            return trap;
        }
    }
} // namespace svm
