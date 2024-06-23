#include "single_core.hpp"

namespace svm
{
    
        Trap SingleCore::XCHG_rm(Regs first, MemoryAddress addr) noexcept
        {
            auto lvalue = readRegister(first);
            auto [trap, rvalue] = mem->read(addr);
            if(trap == Trap::OK)
            {
                std::swap(lvalue, rvalue);
                writeRegister(first, lvalue);
                trap = mem->write(addr, rvalue);
                return trap;
            }
            else
            {
                return trap;
            }
        }
        Trap SingleCore::XCHG_mr(MemoryAddress addr, Regs second) noexcept
        {            
            auto rvalue = readRegister(second);
            auto [trap, lvalue] = mem->read(addr);
            if(trap == Trap::OK)
            {
                std::swap(lvalue, rvalue);
                writeRegister(second, rvalue);
                trap = mem->write(addr, lvalue);
                return trap;
            }
            else
            {
                return trap;
            }
        }
        Trap SingleCore::XCHG_rr(Regs first, Regs second) noexcept
        {
            auto lvalue = readRegister(first);
            auto rvalue = readRegister(second);
            std::swap(lvalue, rvalue);
            writeRegister(first, lvalue);
            writeRegister(second, rvalue);
            return Trap::OK;
        }
} // namespace svm
