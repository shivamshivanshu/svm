#pragma once
#include <cstdint>
#include <cstddef>
#include <array>

#include "trap.hpp"
namespace svm
{

    using MemoryAddress = uint32_t;
    template <typename T, std::size_t N>
    struct Memory
    {
    private:
        std::array<T, N> memory;

    public:
        [[nodiscard]] Trap write(MemoryAddress addr, uint8_t value) noexcept;
        [[nodiscard]] std::pair<Trap, T> read(MemoryAddress addr) const noexcept;
    };

    template <typename T, std::size_t N>
    std::pair<Trap, T> Memory<T, N>::read(MemoryAddress addr) const noexcept
    {
        Trap trap = Trap::ILLEGAL;
        uint8_t value{0};
        if (addr <= N)
        {
            trap = Trap::OK;
            value = memory[addr];
        }
        return {trap, value};
    }

    template<typename T, std::size_t N>
    Trap Memory<T, N>::write(MemoryAddress addr, uint8_t value) noexcept 
    {
        Trap trap = Trap::SEG_FAULT;
        if(addr <= N)
        {
            memory[addr] = value;
            trap = Trap::OK;
        }
        return trap;
    }

} // namespace svm
