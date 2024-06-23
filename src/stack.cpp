#include "single_core.hpp"

namespace svm
{
    Trap SingleCore::POP_m(MemoryAddress addr) noexcept
    {
        Immediate stackSegment = readRegister(Regs::SS), stackPointer = readRegister(Regs::SP);
        MemoryAddress memAddr = static_cast<MemoryAddress>(stackSegment) * 16U + stackPointer;
        if (auto [trap, value] = mem->read(memAddr); trap == Trap::OK)
        {
            trap = mem->write(addr, value);
            stackPointer += sizeof(Register);
            writeRegister(Regs::SP, stackPointer);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    Trap SingleCore::POP_r(Regs first) noexcept
    {
        Immediate stackSegment = readRegister(Regs::SS), stackPointer = readRegister(Regs::SP);
        MemoryAddress memAddr = static_cast<MemoryAddress>(stackSegment) * 16U + stackPointer;
        if (auto [trap, value] = mem->read(memAddr); trap == Trap::OK)
        {
            writeRegister(first, value);
            stackPointer += sizeof(Register);
            writeRegister(Regs::SP, stackPointer);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    Trap SingleCore::POPF() noexcept
    {
        return POP_r(Regs::FLAG);
    }
    Trap SingleCore::POPA() noexcept
    {
        Trap trap = Trap::OK;
        trap = POP_r(Regs::DI);
        trap = POP_r(Regs::BP);
        trap = POP_r(Regs::AX); // Stack Pointer ignored
        trap = POP_r(Regs::BX);
        trap = POP_r(Regs::DX);
        trap = POP_r(Regs::CX);
        trap = POP_r(Regs::AX);
        return trap;
    }
    Trap SingleCore::PUSH_r(Regs first) noexcept
    {
        Immediate stackSegment = readRegister(Regs::SS), stackPointer = readRegister(Regs::SP);
        MemoryAddress memAddr = static_cast<MemoryAddress>(stackSegment) * 16U + stackPointer;
        Register regValue = readRegister(first);
        if (auto trap = mem->write(memAddr, regValue); trap == Trap::OK)
        {
            stackPointer -= sizeof(Register);
            writeRegister(Regs::SP, stackPointer);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    Trap SingleCore::PUSH_i(Immediate value) noexcept
    {
        Immediate stackSegment = readRegister(Regs::SS), stackPointer = readRegister(Regs::SP);
        MemoryAddress memAddr = static_cast<MemoryAddress>(stackSegment) * 16U + stackPointer;
        if (auto trap = mem->write(memAddr, value); trap == Trap::OK)
        {
            stackPointer -= sizeof(Register);
            writeRegister(Regs::SP, stackPointer);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    Trap SingleCore::PUSH_m(MemoryAddress addr) noexcept
    {
        Immediate stackSegment = readRegister(Regs::SS), stackPointer = readRegister(Regs::SP);
        MemoryAddress memAddr = static_cast<MemoryAddress>(stackSegment) * 16U + stackPointer;
        if (auto [trap, value] = mem->read(addr); trap == Trap::OK)
        {
            trap = mem->write(memAddr, value);
            stackPointer -= sizeof(Register);
            writeRegister(Regs::SP, stackPointer);
            return trap;
        }
        else
        {
            return trap;
        }
    }
}