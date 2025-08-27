#pragma once
#include <array>
#include <cstdint>
#include <utility>

#include "arch.hpp"
#include "constants.hpp"
#include "trap.hpp"

namespace svm
{
struct RandomAccessMemory
{
    static constexpr auto Capacity = constants::MAX_MEMORY_CAPACITY;
    static constexpr auto MemoryAlign = constants::MEMORY_BUS_ALIGNMENT;

    RandomAccessMemory() = default;
    [[nodiscard]] Trap write(arch::MemoryAddress aMemoryAddress, arch::Immediate aImmediate) noexcept;
    [[nodiscard]] Trap writeByte(arch::MemoryAddress aMemoryAddress, arch::Immediate aImmediate) noexcept;
    [[nodiscard]] std::pair<Trap, arch::Immediate> read(arch::MemoryAddress aMemoryAddress) const noexcept;
    [[nodiscard]] std::pair<Trap, arch::Immediate> readByte(arch::MemoryAddress aMemoryAddress) const noexcept;

  private:
    bool isMemoryInBound(arch::MemoryAddress) const noexcept;
    std::array<std::uint8_t, constants::MAX_MEMORY_CAPACITY> theMemory;

};
} // namespace svm
