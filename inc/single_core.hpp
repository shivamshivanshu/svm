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
        Register setupFlags(Register, Register, uint32_t) noexcept;
        bool calculateParity(Register)  noexcept;

        // Instruction set

        // AND
        Trap AND_rr(Regs, Regs) noexcept;
        Trap AND_mr(MemoryAddress, Regs) noexcept;
        Trap AND_rm(Regs, MemoryAddress) noexcept;
        Trap AND_mi(MemoryAddress, Immediate) noexcept;
        Trap AND_ri(Regs, Immediate) noexcept;

        void ADC(Operand &op);
        void ADD(Operand &op);
        void CALL(Operand &op);
        void CBW(Operand &op);

        // CLC
        Trap CLC() noexcept;
        // CLD
        Trap CLD() noexcept;
        // CLI
        Trap CLI() noexcept;
        // CMC
        Trap CMC() noexcept;
        // CMP
        void CMP(Operand &op);
        void CMPSB(Operand &op);
        void CMPSW(Operand &op);
        void CWD(Operand &op);
        // DEC
        Trap DEC_m(MemoryAddress) noexcept;
        Trap DEC_r(Regs) noexcept;
        void DIV(Operand &op);
        // HLT
        Trap HLT() noexcept;
        void IDIV(Operand &op);
        void IMUL(Operand &op);
        void IN(Operand &op);
        // INC
        Trap INC_m(MemoryAddress) noexcept;
        Trap INC_r(Regs) noexcept;
        // INT
        Trap INT(Immediate);
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
        Trap MOV_rr(Regs, Regs) noexcept;
        Trap MOV_rm(Regs, MemoryAddress) noexcept;
        Trap MOV_ri(Regs, Immediate) noexcept;
        Trap MOV_mr(MemoryAddress, Regs) noexcept;
        Trap MOV_mi(MemoryAddress, Immediate) noexcept;

        void MOVSB(Operand &op);
        void MOVSW(Operand &op);
        void MUL(Operand &op);

        // NEG
        Trap NEG_r(Regs) noexcept;
        Trap NEG_m(MemoryAddress) noexcept;

        // NOP
        Trap NOP() noexcept;

        // OR
        Trap OR_rr(Regs, Regs) noexcept;
        Trap OR_rm(Regs, MemoryAddress) noexcept;
        Trap OR_ri(Regs, Immediate) noexcept;
        Trap OR_mr(MemoryAddress, Regs) noexcept;
        Trap OR_mi(MemoryAddress, Immediate) noexcept;

        void OUT(Operand &op);

        // POP
        Trap POP_r(Regs) noexcept;
        Trap POP_m(MemoryAddress) noexcept;
        //POPA
        Trap POPA() noexcept;
        //POPF
        Trap POPF() noexcept;
        // PUSH
        Trap PUSH_r(Regs) noexcept;
        Trap PUSH_m(MemoryAddress) noexcept;
        Trap PUSH_i(Immediate) noexcept;

        Trap PUSHA() noexcept;
        Trap PUSHF() noexcept;
        void ROL(Operand &op);
        void ROR(Operand &op);
        void SAHF(Operand &op);
        void SAL(Operand &op);
        void SAR(Operand &op);
        void SBB(Operand &op);
        void SCASB(Operand &op);
        void SCASW(Operand &op);

        Trap STC() noexcept;
        Trap STD() noexcept;
        Trap STI() noexcept;

        void STOSB(Operand &op);
        void STOSW(Operand &op);
        void SUB(Operand &op);
        void TEST(Operand &op);

        // XCHG
        Trap XCHG_rm(Regs, MemoryAddress) noexcept;
        Trap XCHG_mr(MemoryAddress, Regs) noexcept;
        Trap XCHG_rr(Regs, Regs) noexcept;

        void XLATB(Operand &op);

        // XOR
        Trap XOR_rr(Regs, Regs) noexcept;
        Trap XOR_rm(Regs, MemoryAddress) noexcept;
        Trap XOR_ri(Regs, Immediate) noexcept;
        Trap XOR_mr(MemoryAddress, Regs) noexcept;
        Trap XOR_mi(MemoryAddress, Immediate) noexcept;
    };
}