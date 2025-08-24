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
  private:
    std::array<std::uint8_t, constants::MAX_MEMORY_CAPACITY> theMemory;

  public:
    [[nodiscard]] Trap write(arch::MemoryAddress aMemoryAddress, arch::Immediate aImmediate) noexcept;
    [[nodiscard]] std::pair<Trap, arch::Immediate> read(arch::MemoryAddress aMemoryAddress) const noexcept;
};
} // namespace svm
