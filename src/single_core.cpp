#include "single_core.hpp"
#include "arch.hpp"
#include "memory"

namespace svm
{
    SingleCore::SingleCore()
    {
        mem = nullptr;

        // General Purpose Register
        reg[Regs::AX] = {0, 0};
        reg[Regs::BX] = {0, 0};
        reg[Regs::CX] = {0, 0};
        reg[Regs::DX] = {0, 0};

        // Segment Registers
        reg[Regs::CS] = {0, 0};
        reg[Regs::DS] = {0, 0};
        reg[Regs::SS] = {0, 0};
        reg[Regs::ES] = {0, 0};

        // Pointer and Index Registers
        reg[Regs::SP] = {0, 0};
        reg[Regs::BP] = {0, 0};
        reg[Regs::SI] = {0, 0};
        reg[Regs::DI] = {0, 0};

        // Special Purpose Register
        reg[Regs::IP] = {0, 0};
        reg[Regs::FLAG] = {0, 0};
    }

    SingleCore::SingleCore(std::shared_ptr<Memory> ptr) : SingleCore()
    {
        mem = ptr;
    }

    T SingleCore::readRegister(Regs r) noexcept
    {
        T value = (static_cast<T>(reg.at(r).first) << RegisterHalfSize) + (static_cast<T>(reg.at(r).second));
        return value;
    }

    void SingleCore::writeRegister(Regs r, Immediate value) noexcept
    {
        reg[r].first = static_cast<RegisterHalf>((value & upperHalfMask) >> RegisterHalfSize);
        reg[r].second = static_cast<RegisterHalf>(value & lowerHalfMask);
    }

} // namespace svm
