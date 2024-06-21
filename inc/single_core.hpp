#pragma once
#include <memory>

#include "memory.hpp"
#include "trap.hpp"
#include "arch.hpp"

namespace svm
{
    struct SingleCore
    {
        Registers reg;
        std::shared_ptr<Memory> mem;

        // Constructors
        SingleCore();
        ~SingleCore() = default;
        SingleCore(const SingleCore &) = delete;
        SingleCore(SingleCore &&) = delete;
        SingleCore &operator=(const SingleCore &) = delete;

        SingleCore(std::shared_ptr<Memory> memory);

        void parseInstruction() noexcept;
        [[nodiscard]] Register readRegister(Regs) noexcept;
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

        // NEG
        Trap NEG_r(Regs);
        Trap NEG_m(MemoryAddress);

        void NOP(Operand &op);

        // OR
        Trap OR_rr(Regs, Regs);
        Trap OR_rm(Regs, MemoryAddress);
        Trap OR_ri(Regs, Immediate);
        Trap OR_mr(MemoryAddress, Regs);
        Trap OR_mi(MemoryAddress, Immediate);

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

        // XOR
        Trap XOR_rr(Regs, Regs);
        Trap XOR_rm(Regs, MemoryAddress);
        Trap XOR_ri(Regs, Immediate);
        Trap XOR_mr(MemoryAddress, Regs);
        Trap XOR_mi(MemoryAddress, Immediate);
    };
}