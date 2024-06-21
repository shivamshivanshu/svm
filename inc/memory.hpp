#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <utility>

#include "trap.hpp"
#include "arch.hpp"
namespace svm
{
    struct Memory
    {
    private:
        std::array<T, N> memory;

    public:
        [[nodiscard]] Trap write(MemoryAddress addr, Immediate value) noexcept;
        [[nodiscard]] std::pair<Trap, T> read(MemoryAddress addr) const noexcept;
    };
} // namespace svm