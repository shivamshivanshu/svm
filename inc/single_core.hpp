#pragma once
#include <memory>

#include "arch.hpp"
#include "memory.hpp"
#include "trap.hpp"

namespace svm
{
struct SingleCore
{

    // Constructors
    ~SingleCore() = default;
    SingleCore(const SingleCore &) = delete;
    SingleCore(SingleCore &&) = delete;
    SingleCore &operator=(const SingleCore &) = delete;

    SingleCore(RandomAccessMemory &aMemory);

    enum class BinaryOp
    {
        Add,
        Sub,
    };

    // Instruction set
    Trap AAA(void) noexcept;
    Trap AAD(void) noexcept;
    Trap AAM(void) noexcept;
    Trap AAS(void) noexcept;

    Trap ADC(arch::Regs, arch::Regs) noexcept;
    Trap ADC(arch::Regs, arch::MemoryAddress) noexcept;
    Trap ADC(arch::Regs, arch::Immediate) noexcept;
    Trap ADC(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap ADC(arch::MemoryAddress, arch::Regs) noexcept;

    Trap AND(arch::Regs, arch::Regs) noexcept;
    Trap AND(arch::Regs, arch::MemoryAddress) noexcept;
    Trap AND(arch::Regs, arch::Immediate) noexcept;
    Trap AND(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap AND(arch::MemoryAddress, arch::Regs) noexcept;
    void AND_setFlags(arch::Immediate) noexcept;

    Trap CALL(arch::MemoryAddress) noexcept;
    Trap CALL(arch::Regs) noexcept;

    Trap CBW(void) noexcept;

    Trap CLC(void) noexcept;
    Trap CLD(void) noexcept;
    Trap CLI(void) noexcept;

    Trap CMC(void) noexcept;

    Trap CMP(arch::Regs, arch::Regs) noexcept;
    Trap CMP(arch::Regs, arch::MemoryAddress) noexcept;
    Trap CMP(arch::Regs, arch::Immediate) noexcept;
    Trap CMP(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap CMP(arch::MemoryAddress, arch::Regs) noexcept;

    Trap CMPSB(void) noexcept;
    Trap CMPSW(void) noexcept;
    Trap CWD(void) noexcept;

    Trap DAA(void) noexcept;
    Trap DAS(void) noexcept;

    Trap DEC(arch::Regs) noexcept;
    Trap DEC(arch::MemoryAddress) noexcept;

    Trap DIV(arch::Regs) noexcept;
    Trap DIV(arch::MemoryAddress) noexcept;

    Trap HLT(void) noexcept;

    Trap IDIV(arch::Regs) noexcept;
    Trap IDIV(arch::MemoryAddress) noexcept;

    Trap IMUL(arch::Regs) noexcept;
    Trap IMUL(arch::MemoryAddress) noexcept;

    Trap IN(arch::Immediate, arch::RegLevel) noexcept;
    Trap IN(arch::RegLevel) noexcept;

    Trap INC(arch::Regs) noexcept;
    Trap INC(arch::MemoryAddress) noexcept;

    Trap INT(arch::Immediate) noexcept;
    Trap INTO(void) noexcept;
    Trap IRET(void) noexcept;

    Trap JA(arch::MemoryAddress) noexcept;   // TODO
    Trap JAE(arch::MemoryAddress) noexcept;  // TODO
    Trap JB(arch::MemoryAddress) noexcept;   // TODO
    Trap JBE(arch::MemoryAddress) noexcept;  // TODO
    Trap JC(arch::MemoryAddress) noexcept;   // TODO
    Trap JCXZ(arch::MemoryAddress) noexcept; // TODO
    Trap JE(arch::MemoryAddress) noexcept;   // TODO
    Trap JG(arch::MemoryAddress) noexcept;   // TODO
    Trap JGE(arch::MemoryAddress) noexcept;  // TODO
    Trap JL(arch::MemoryAddress) noexcept;   // TODO
    Trap JMP(arch::MemoryAddress) noexcept;  // TODO
    Trap JNA(arch::MemoryAddress) noexcept;  // TODO
    Trap JNAE(arch::MemoryAddress) noexcept; // TODO
    Trap JNB(arch::MemoryAddress) noexcept;  // TODO
    Trap JNBE(arch::MemoryAddress) noexcept; // TODO
    Trap JNC(arch::MemoryAddress) noexcept;  // TODO
    Trap JNE(arch::MemoryAddress) noexcept;  // TODO
    Trap JNG(arch::MemoryAddress) noexcept;  // TODO
    Trap JNGE(arch::MemoryAddress) noexcept; // TODO
    Trap JNL(arch::MemoryAddress) noexcept;  // TODO
    Trap JNLE(arch::MemoryAddress) noexcept; // TODO
    Trap JNO(arch::MemoryAddress) noexcept;  // TODO
    Trap JNP(arch::MemoryAddress) noexcept;  // TODO
    Trap JNS(arch::MemoryAddress) noexcept;  // TODO
    Trap JNZ(arch::MemoryAddress) noexcept;  // TODO
    Trap JO(arch::MemoryAddress) noexcept;   // TODO
    Trap JP(arch::MemoryAddress) noexcept;   // TODO
    Trap JPE(arch::MemoryAddress) noexcept;  // TODO
    Trap JPO(arch::MemoryAddress) noexcept;  // TODO
    Trap JZ(arch::MemoryAddress) noexcept;   // TODO

    Trap LAHF(void) noexcept;
    Trap LDS(arch::Regs, arch::MemoryAddress) noexcept;
    Trap LEA(arch::Regs, arch::MemoryAddress) noexcept;
    Trap LES(arch::Regs, arch::MemoryAddress) noexcept;

    Trap LODSB(void) noexcept;
    Trap LODSW(void) noexcept;
    Trap LOOP(arch::MemoryAddress) noexcept;   // TODO
    Trap LOOPE(arch::MemoryAddress) noexcept;  // TODO
    Trap LOOPNE(arch::MemoryAddress) noexcept; // TODO
    Trap LOOPNZ(arch::MemoryAddress) noexcept; // TODO
    Trap LOOPZ(arch::MemoryAddress) noexcept;  // TODO

    Trap MOV(arch::Regs, arch::Regs) noexcept;
    Trap MOV(arch::Regs, arch::MemoryAddress) noexcept;
    Trap MOV(arch::Regs, arch::Immediate) noexcept;
    Trap MOV(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap MOV(arch::MemoryAddress, arch::Regs) noexcept;

    Trap MOVSB(void) noexcept;
    Trap MOVSW(void) noexcept;

    Trap MUL(arch::Regs, arch::MemoryAddress) noexcept;
    Trap NEG(arch::Regs, arch::MemoryAddress) noexcept;
    Trap NOP(void) noexcept;
    Trap NOT(arch::Regs, arch::MemoryAddress) noexcept;

    Trap OR(arch::Regs, arch::Regs) noexcept;
    Trap OR(arch::Regs, arch::MemoryAddress) noexcept;
    Trap OR(arch::Regs, arch::Immediate) noexcept;
    Trap OR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap OR(arch::MemoryAddress, arch::Regs) noexcept;

    Trap OUT(arch::Immediate, arch::Regs, arch::RegLevel) noexcept;
    Trap OUT(arch::RegLevel) noexcept;

    Trap POP(arch::Regs) noexcept;
    Trap POP(arch::MemoryAddress) noexcept;

    Trap POPA(void) noexcept;
    Trap POPF(void) noexcept;

    Trap PUSH(arch::Regs) noexcept;
    Trap PUSH(arch::MemoryAddress) noexcept;
    Trap PUSH(arch::Immediate) noexcept;
    Trap PUSHF(void) noexcept;

    Trap RCL(arch::Regs) noexcept;
    Trap RCL(arch::MemoryAddress) noexcept;
    Trap RCL(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap RCL(arch::Regs, arch::Immediate) noexcept;

    Trap RCR(arch::Regs) noexcept;
    Trap RCR(arch::MemoryAddress) noexcept;
    Trap RCR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap RCR(arch::Regs, arch::Immediate) noexcept;

    Trap REP(void) noexcept;   // TODO
    Trap REPE(void) noexcept;  // TODO
    Trap REPNE(void) noexcept; // TODO
    Trap REPNZ(void) noexcept; // TODO
    Trap REPZ(void) noexcept;  // TODO

    Trap RET(void) noexcept;
    Trap RET(arch::Immediate) noexcept;

    Trap RETF(void) noexcept;
    Trap RETF(arch::Immediate) noexcept;

    Trap ROL(arch::Regs) noexcept;
    Trap ROL(arch::MemoryAddress) noexcept;
    Trap ROL(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap ROL(arch::Regs, arch::Immediate) noexcept;

    Trap ROR(arch::Regs) noexcept;
    Trap ROR(arch::MemoryAddress) noexcept;
    Trap ROR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap ROR(arch::Regs, arch::Immediate) noexcept;

    Trap SAHF(void) noexcept;

    Trap SAL(arch::Regs) noexcept;
    Trap SAL(arch::MemoryAddress) noexcept;
    Trap SAL(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SAL(arch::Regs, arch::Immediate) noexcept;

    Trap SAR(arch::Regs) noexcept;
    Trap SAR(arch::MemoryAddress) noexcept;
    Trap SAR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SAR(arch::Regs, arch::Immediate) noexcept;

    Trap SBB(arch::Regs, arch::Regs) noexcept;
    Trap SBB(arch::Regs, arch::MemoryAddress) noexcept;
    Trap SBB(arch::Regs, arch::Immediate) noexcept;
    Trap SBB(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SBB(arch::MemoryAddress, arch::Regs) noexcept;

    Trap SCASB(arch::Regs) noexcept;
    Trap SCASW(arch::Regs) noexcept;

    Trap SHL(arch::Regs) noexcept;
    Trap SHL(arch::MemoryAddress) noexcept;
    Trap SHL(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SHL(arch::Regs, arch::Immediate) noexcept;

    Trap SHR(arch::Regs) noexcept;
    Trap SHR(arch::MemoryAddress) noexcept;
    Trap SHR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SHR(arch::Regs, arch::Immediate) noexcept;

    Trap STC(void) noexcept;
    Trap STD(void) noexcept;
    Trap STI(void) noexcept;
    Trap STOSB(void) noexcept;
    Trap STOSW(void) noexcept;

    Trap SUB(arch::Regs, arch::Regs) noexcept;
    Trap SUB(arch::Regs, arch::MemoryAddress) noexcept;
    Trap SUB(arch::Regs, arch::Immediate) noexcept;
    Trap SUB(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap SUB(arch::MemoryAddress, arch::Regs) noexcept;

    Trap TEST(arch::Regs, arch::Regs) noexcept;
    Trap TEST(arch::Regs, arch::MemoryAddress) noexcept;
    Trap TEST(arch::Regs, arch::Immediate) noexcept;
    Trap TEST(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap TEST(arch::MemoryAddress, arch::Regs) noexcept;

    Trap XCHG(arch::Regs, arch::Regs) noexcept;
    Trap XCHG(arch::Regs, arch::MemoryAddress) noexcept;
    Trap XCHG(arch::MemoryAddress, arch::Regs) noexcept;

    Trap XLATB(void) noexcept;
    Trap XOR(arch::Regs, arch::Regs) noexcept;
    Trap XOR(arch::Regs, arch::MemoryAddress) noexcept;
    Trap XOR(arch::Regs, arch::Immediate) noexcept;
    Trap XOR(arch::MemoryAddress, arch::Immediate) noexcept;
    Trap XOR(arch::MemoryAddress, arch::Regs) noexcept;

    void parseInstruction() noexcept;
    arch::Register &getReg(arch::Regs) noexcept;
    [[nodiscard]] arch::Immediate readRegister(arch::Regs) noexcept;
    void writeRegister(arch::Regs, arch::Immediate) noexcept;
    void setFlag(arch::Flags, arch::Immediate) noexcept;
    arch::Immediate readFlag(arch::Flags) noexcept;
    arch::MemoryAddress getEffectiveAddr(arch::Regs, arch::Regs) const noexcept;

  private:
    template<BinaryOp Op>
    arch::Immediate computeArithmeticFlags(std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
    arch::Immediate setFlagOnAdd(std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
    arch::Immediate setFlagOnCmp(std::uint32_t, std::uint32_t) noexcept;
    // General Purpose Registers
    arch::Register theAX{arch::Register{.theLabel = arch::Regs::AX, .theRegisterValue = 0}};
    arch::Register theBX{arch::Register{.theLabel = arch::Regs::BX, .theRegisterValue = 0}};
    arch::Register theCX{arch::Register{.theLabel = arch::Regs::CX, .theRegisterValue = 0}};
    arch::Register theDX{arch::Register{.theLabel = arch::Regs::DX, .theRegisterValue = 0}};

    // Segment Registers
    arch::Register theCS{arch::Register{.theLabel = arch::Regs::CS, .theRegisterValue = 0}};
    arch::Register theDS{arch::Register{.theLabel = arch::Regs::DS, .theRegisterValue = 0}};
    arch::Register theSS{arch::Register{.theLabel = arch::Regs::SS, .theRegisterValue = 0}};
    arch::Register theES{arch::Register{.theLabel = arch::Regs::ES, .theRegisterValue = 0}};

    // Pointer and Index Registers
    arch::Register theSP{arch::Register{.theLabel = arch::Regs::SP, .theRegisterValue = 0}};
    arch::Register theBP{arch::Register{.theLabel = arch::Regs::BP, .theRegisterValue = 0}};
    arch::Register theSI{arch::Register{.theLabel = arch::Regs::SI, .theRegisterValue = 0}};
    arch::Register theDI{arch::Register{.theLabel = arch::Regs::DI, .theRegisterValue = 0}};

    // Instruction Pointer
    arch::Register theIP{arch::Register{.theLabel = arch::Regs::IP, .theRegisterValue = 0}};
    arch::Register theFlag{arch::Register{.theLabel = arch::Regs::FLAG, .theRegisterValue = 0}};

    RandomAccessMemory &theMemory;
};
} // namespace svm
