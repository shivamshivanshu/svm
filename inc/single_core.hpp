#pragma once
#include <memory>

#include "memory.hpp"
#include "trap.hpp"
#include "instructions.hpp"

namespace svm
{

    template <typename T, std::size_t N>
    struct SingleCore
    {
        Registers reg;
        std::shared_ptr<Memory<T, N>> mem;

        // Constructors
        SingleCore();
        ~SingleCore() = default;
        SingleCore(const SingleCore &) = delete;
        SingleCore(SingleCore &&) = delete;
        SingleCore &operator=(const SingleCore &) = delete;

        SingleCore(std::shared_ptr<Memory<T, N>> memory);

        void parseInstruction() noexcept;
        [[nodiscard]] T readRegister(Regs) noexcept;
        void writeRegister(Regs, Immediate) noexcept;

        // Instruction set

        // AND
        Trap AND_rr(Regs, Regs);
        Trap AND_mr(MemoryAddress, Regs);
        Trap AND_rm(Regs, MemoryAddress);
        Trap AND_mi(MemoryAddress, Immediate);
        Trap AND_ri(Regs, Immediate);

        void ADC(Operand &op);
        void ADD(Operand &op);
        void CALL(Operand &op);
        void CBW(Operand &op);
        void CLD(Operand &op);
        void CLI(Operand &op);
        void CMC(Operand &op);
        void CMP(Operand &op);
        void CMPSB(Operand &op);
        void CMPSW(Operand &op);
        void CWD(Operand &op);
        void DEC(Operand &op);
        void DIV(Operand &op);
        void HLT(Operand &op);
        void IDIV(Operand &op);
        void IMUL(Operand &op);
        void IN(Operand &op);
        void INC(Operand &op);
        void INT(Operand &op);
        void INTO(Operand &op);
        void IRET(Operand &op);
        void JMP(Operand &op);
        void LAHF(Operand &op);
        void LDS(Operand &op);
        void LEA(Operand &op);
        void LES(Operand &op);
        void LODSB(Operand &op);
        void LODSW(Operand &op);

        // MOV
        Trap MOV_rr(Regs, Regs);
        Trap MOV_rm(Regs, MemoryAddress);
        Trap MOV_ri(Regs, Immediate);
        Trap MOV_mr(MemoryAddress, Regs);
        Trap MOV_mi(MemoryAddress, Immediate);

        void MOVSB(Operand &op);
        void MOVSW(Operand &op);
        void MUL(Operand &op);
        void NEG(Operand &op);
        void NOP(Operand &op);
        void OR(Operand &op);
        void OUT(Operand &op);
        void POP(Operand &op);
        void POPA(Operand &op);
        void POPF(Operand &op);
        void PUSH(Operand &op);
        void PUSHA(Operand &op);
        void PUSHF(Operand &op);
        void ROL(Operand &op);
        void ROR(Operand &op);
        void SAHF(Operand &op);
        void SAL(Operand &op);
        void SAR(Operand &op);
        void SBB(Operand &op);
        void SCASB(Operand &op);
        void SCASW(Operand &op);
        void STC(Operand &op);
        void STD(Operand &op);
        void STI(Operand &op);
        void STOSB(Operand &op);
        void STOSW(Operand &op);
        void SUB(Operand &op);
        void TEST(Operand &op);
        void XCHG(Operand &op);
        void XLATB(Operand &op);
        void XOR(Operand &op);
    };
}

namespace svm
{
    template <typename T, std::size_t N>
    SingleCore<T, N>::SingleCore()
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
    template <typename T, std::size_t N>
    SingleCore<T, N>::SingleCore(std::shared_ptr<Memory<T, N>> ptr) : SingleCore()
    {
        mem = ptr;
    }

    template <typename T, std::size_t N>
    T SingleCore<T, N>::readRegister(Regs r) noexcept
    {
        T value = (static_cast<T>(reg.at(r).first) << RegisterHalfSize) + (static_cast<T>(reg.at(r).second));
        return value;
    }

    template <typename T, std::size_t N>
    void SingleCore<T, N>::writeRegister(Regs r, Immediate value) noexcept
    {
        reg[r].first = static_cast<RegisterHalf>((value & upperHalfMask) >> RegisterHalfSize);
        reg[r].second = static_cast<RegisterHalf>(value & lowerHalfMask);
    }
}

namespace svm // AND
{
    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::AND_rr(Regs first, Regs second)
    {
        std::pair<RegisterHalf, RegisterHalf> &valFirst = reg[first], &valSecond = reg[second];
        valFirst.first &= valSecond.first;
        valFirst.second &= valSecond.second;
        return Trap::OK;
    }

    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::AND_mr(MemoryAddress addr, Regs second)
    {
        auto [trap, lvalue] = mem->read(addr);
        T rvalue = readRegister(second);
        if (trap == Trap::OK)
        {
            lvalue &= rvalue;
            trap = mem->write(addr, lvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }

    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::AND_rm(Regs first, MemoryAddress addr)
    {
        auto regValue = readRegister(first);
        auto [trap, rvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            regValue &= rvalue;
            writeRegister(first, regValue);
            return trap;
        }
        else
        {
            return trap;
        }
    }
    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::AND_ri(Regs first, Immediate value)
    {
        auto regValue = readRegister(first);
        regValue &= value;
        writeRegister(first, regValue);
        return Trap::OK;
    }
    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::AND_mi(MemoryAddress addr, Immediate value)
    {
        auto [trap, lvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            lvalue &= value;
            trap = mem->write(addr, lvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }
}

namespace svm // MOV
{
    template <typename T, std::size_t N>
    Trap SingleCore<T, N>::MOV_rr(Regs first, Regs second)
    {
        auto rvalue = readRegister(second);
        writeRegister(first, rvalue);
        return Trap::OK;
    }

    template<typename T, std::size_t N>
    Trap SingleCore<T, N>::MOV_rm(Regs first, MemoryAddress addr)
    {
        auto [trap, rvalue] = mem->read(addr);
        if (trap == Trap::OK)
        {
            writeRegister(first, rvalue);
            return trap;
        }
        else
        {
            return trap;
        }
    }

    template<typename T, std::size_t N>
    Trap SingleCore<T, N>::MOV_ri(Regs first, Immediate value)
    {
        writeRegister(first, value);
        return Trap::OK;
    }

    template<typename T, std::size_t N>
    Trap SingleCore<T, N>::MOV_mr(MemoryAddress addr, Regs second)
    {
        auto rvalue = readRegister(second);
        auto trap = mem->write(addr, rvalue);
        return trap;
    }

    template<typename T, std::size_t N>
    Trap SingleCore<T, N>::MOV_mi(MemoryAddress addr, Immediate value)
    {
        auto trap = mem->write(addr, value);
        return trap;
    }
}