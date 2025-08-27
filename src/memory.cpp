#include "memory.hpp"
#include "arch.hpp"
#include "constants.hpp"
#include "trap.hpp"

namespace svm
{
std::pair<Trap, arch::Immediate> RandomAccessMemory::read(arch::MemoryAddress aMemoryAddress) const noexcept
{
    if (isMemoryInBound(aMemoryAddress))
    {
        arch::Immediate myReadValue{};
        for (std::size_t i{}; i < RandomAccessMemory::MemoryAlign; ++i)
        {
            myReadValue |= theMemory[aMemoryAddress.theAddress + i]
                           << (constants::CHAR_SIZE * (RandomAccessMemory::MemoryAlign - 1 - i));
        }
        return {Trap::OK, myReadValue};
    }
    else
    {
        return {Trap::SEG_FAULT, 0};
    }
}

std::pair<Trap, arch::Immediate> RandomAccessMemory::readByte(arch::MemoryAddress aMemoryAddress) const noexcept
{
    if (isMemoryInBound(aMemoryAddress))
    {
        arch::Immediate myReadValue = theMemory[aMemoryAddress.theAddress];
        return {Trap::OK, myReadValue};
    }
    else
    {
        return {Trap::SEG_FAULT, 0};
    }
}

bool RandomAccessMemory::isMemoryInBound(arch::MemoryAddress aMemoryAddress) const noexcept
{
    return aMemoryAddress.theAddress + RandomAccessMemory::MemoryAlign < RandomAccessMemory::Capacity;
}

Trap RandomAccessMemory::write(arch::MemoryAddress aMemoryAddress, arch::Immediate aValue) noexcept
{
    if (isMemoryInBound(aMemoryAddress))
    {
        for (std::size_t i{}; i < RandomAccessMemory::MemoryAlign; ++i)
        {
            const std::size_t myShift = constants::CHAR_SIZE * (RandomAccessMemory::MemoryAlign - 1 - i);
            theMemory[aMemoryAddress.theAddress + i] = (aValue >> myShift) & constants::BYTE_MASK;
        }
        return Trap::OK;
    }
    else
    {
        return Trap::SEG_FAULT;
    }
}

Trap RandomAccessMemory::writeByte(arch::MemoryAddress aMemoryAddress, arch::Immediate aValue) noexcept
{
    if (isMemoryInBound(aMemoryAddress))
    {
        theMemory[aMemoryAddress.theAddress] = aValue;
        return Trap::OK;
    }
    else
    {
        return Trap::SEG_FAULT;
    }
}
} // namespace svm
