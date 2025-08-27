#include <utility>

#include "arch.hpp"
#include "constants.hpp"
#include "single_core.hpp"
#include "single_core_util.hpp"
#include "trap.hpp"

namespace svm
{
SingleCore::SingleCore(RandomAccessMemory &aMemory) : theMemory{aMemory}
{
}

arch::Register &SingleCore::getReg(arch::Regs aRegister) noexcept
{
    switch (aRegister)
    {
    // General Registers
    case arch::Regs::AX:
        return theAX;
    case arch::Regs::BX:
        return theBX;
    case arch::Regs::CX:
        return theCX;
    case arch::Regs::DX:
        return theDX;

    // Segment Registers
    case arch::Regs::CS:
        return theCS;
    case arch::Regs::DS:
        return theDS;
    case arch::Regs::SS:
        return theSS;
    case arch::Regs::ES:
        return theES;

    // Stack Registers
    case arch::Regs::SP:
        return theSP;
    case arch::Regs::BP:
        return theBP;
    case arch::Regs::SI:
        return theSI;
    case arch::Regs::DI:
        return theDI;

    // Instruction Registers
    case arch::Regs::IP:
        return theIP;

    // Flag Register
    case arch::Regs::FLAG:
        return theFlag;

    default:
        std::unreachable();
    }
}

void SingleCore::writeRegister(arch::Regs aRegister, arch::Immediate aValue) noexcept
{
    auto &myRegister = getReg(aRegister);
    myRegister.theRegisterValue = aValue;
}

arch::Immediate SingleCore::readRegister(arch::Regs aRegister) noexcept
{
    const auto &myRegister = getReg(aRegister);
    return myRegister.theRegisterValue;
}

void SingleCore::setFlag(arch::Flags aFlag, arch::Immediate aValue) noexcept
{
    const auto myFlagLocation = std::to_underlying(aFlag);
    const arch::Immediate myFlagMask = 1 << myFlagLocation;
    theFlag.theRegisterValue =
        (theFlag.theRegisterValue & ~myFlagMask) | (static_cast<arch::Immediate>(aValue) << myFlagLocation);
}

arch::Immediate SingleCore::readFlag(arch::Flags aFlag) noexcept
{
    const auto myFlagLocation = std::to_underlying(aFlag);
    const arch::Immediate myFlagMask = 1 << myFlagLocation;
    return (theFlag.theRegisterValue & myFlagMask) >> myFlagLocation;
}

Trap SingleCore::AAA(void) noexcept
{
    const auto myAxValue = theAX.theRegisterValue;
    arch::Immediate myAL = myAxValue & 0xFF;
    arch::Immediate myAH = (myAxValue >> 8) & 0xFF;
    if ((myAL & 0x0F) > 9 || readFlag(arch::Flags::AF))
    {
        myAL += 6;
        myAH += 1;

        setFlag(arch::Flags::AF, 1);
        setFlag(arch::Flags::CF, 1);
    }
    else
    {
        setFlag(arch::Flags::AF, 0);
        setFlag(arch::Flags::CF, 0);
    }
    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xF);
    theAX.theRegisterValue = myAx;
    return Trap::OK;
}

Trap SingleCore::AAD(void) noexcept
{
    const auto myAxValue = theAX.theRegisterValue;
    arch::Immediate myAL = myAxValue & 0xFF;
    arch::Immediate myAH = (myAxValue >> 8) & 0xFF;

    myAL += myAH * 10;
    myAH = 0;

    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xFF);
    theAX.theRegisterValue = myAx;
    return Trap::OK;
}

Trap SingleCore::AAM(void) noexcept
{
    const auto myAxValue = theAX.theRegisterValue;
    arch::Immediate myAL = myAxValue & 0xFF;
    arch::Immediate myAH = myAL / 10;
    myAL = myAL % 10;

    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xFF);
    theAX.theRegisterValue = myAx;
    return Trap::OK;
}

Trap SingleCore::AAS(void) noexcept
{
    const auto myAxValue = theAX.theRegisterValue;
    arch::Immediate myAL = myAxValue & 0xFF;
    arch::Immediate myAH = (myAxValue >> 8) & 0xFF;
    if ((myAL & 0x0F) > 9 || readFlag(arch::Flags::AF))
    {
        myAL -= 6;
        myAH -= 1;
        setFlag(arch::Flags::AF, 1);
        setFlag(arch::Flags::CF, 1);
    }
    else
    {
        setFlag(arch::Flags::AF, 0);
        setFlag(arch::Flags::CF, 0);
    }

    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xF);
    theAX.theRegisterValue = myAx;
    return Trap::OK;
}

arch::Immediate SingleCore::setFlagOnAdd(std::uint32_t aSource, std::uint32_t aDest, std::uint32_t aCarry) noexcept
{
    return computeArithmeticFlags<SingleCore::BinaryOp::Add>(aSource, aDest, aCarry);
}

Trap SingleCore::ADC(arch::Regs aRegister, arch::MemoryAddress aMemory) noexcept
{
    auto &myRegister = getReg(aRegister);
    const auto [myTrap, myMemoryValue] = theMemory.read(aMemory);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    const std::uint32_t myFlag = readFlag(arch::Flags::CF);
    const auto myResult = setFlagOnAdd(myRegister.theRegisterValue, myMemoryValue, myFlag);
    myRegister.theRegisterValue = myResult;
    return Trap::OK;
}

Trap SingleCore::ADC(arch::Regs aFirst, arch::Regs aSecond) noexcept
{
    auto &myFirstyRegister = getReg(aFirst);
    auto &mySecondRegister = getReg(aSecond);
    const auto myFlag = readFlag(arch::Flags::CF);

    myFirstyRegister.theRegisterValue =
        setFlagOnAdd(myFirstyRegister.theRegisterValue, mySecondRegister.theRegisterValue, myFlag);
    return Trap::OK;
}

Trap SingleCore::ADC(arch::Regs aFirst, arch::Immediate aSecond) noexcept
{
    auto &myFirstyRegister = getReg(aFirst);
    const auto myFlag = readFlag(arch::Flags::CF);

    myFirstyRegister.theRegisterValue = setFlagOnAdd(myFirstyRegister.theRegisterValue, aSecond, myFlag);
    return Trap::OK;
}

Trap SingleCore::ADC(arch::MemoryAddress aFirst, arch::Immediate aSecond) noexcept
{
    const auto [myTrap, myValue] = theMemory.read(aFirst);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    const auto myFlag = readFlag(arch::Flags::CF);

    const arch::Immediate myResult = setFlagOnAdd(myValue, aSecond, myFlag);
    return theMemory.write(aFirst, myResult);
}

Trap SingleCore::ADC(arch::MemoryAddress aFirst, arch::Regs aSecond) noexcept
{
    const auto [myTrap, myValue] = theMemory.read(aFirst);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    const auto myFlag = readFlag(arch::Flags::CF);
    const auto myRegister = getReg(aSecond).theRegisterValue;
    const arch::Immediate myResult = setFlagOnAdd(myValue, myRegister, myFlag);

    return theMemory.write(aFirst, myResult);
}

Trap SingleCore::STC(void) noexcept
{
    setFlag(svm::arch::Flags::CF, 1U);
    return Trap::OK;
}

Trap SingleCore::STD(void) noexcept
{
    setFlag(svm::arch::Flags::DF, 1U);
    return Trap::OK;
}

Trap SingleCore::STI(void) noexcept
{
    setFlag(svm::arch::Flags::IF, 1U);
    return Trap::OK;
}

Trap SingleCore::CLC(void) noexcept
{
    setFlag(svm::arch::Flags::CF, 0U);
    return Trap::OK;
}

Trap SingleCore::CLD(void) noexcept
{
    setFlag(svm::arch::Flags::DF, 0U);
    return Trap::OK;
}

Trap SingleCore::CLI(void) noexcept
{
    setFlag(svm::arch::Flags::IF, 0U);
    return Trap::OK;
}

Trap SingleCore::CMC(void) noexcept
{
    const auto myFlag = readFlag(svm::arch::Flags::CF);
    setFlag(svm::arch::Flags::CF, myFlag ^ 1);
    return Trap::OK;
}

void SingleCore::AND_setFlags(arch::Immediate aImmediate) noexcept
{
    const bool myIsCarryFlag = false;
    const bool myIsOverflowFlag = false;
    const bool myIsSignFlag = (aImmediate & 0x8000) != 0;
    const bool myIsZeroFlag = (aImmediate & 0xFFFF) == 0;
    const bool myIsParityFlag = SingleCoreUtil::parity(aImmediate & 0xFF);

    setFlag(arch::Flags::CF, myIsCarryFlag);
    setFlag(arch::Flags::OF, myIsOverflowFlag);
    setFlag(arch::Flags::SF, myIsSignFlag);
    setFlag(arch::Flags::ZF, myIsZeroFlag);
    setFlag(arch::Flags::PF, myIsParityFlag);
}

Trap SingleCore::AND(arch::Regs aFirst, arch::Regs aSecond) noexcept
{
    const auto mySecond = readRegister(aSecond);
    return AND(aFirst, mySecond);
}

Trap SingleCore::AND(arch::Regs aFirst, arch::MemoryAddress aSecond) noexcept
{
    const auto [myTrap, myMemoryValue] = theMemory.read(aSecond);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    return AND(aFirst, static_cast<arch::Immediate>(myMemoryValue));
}

Trap SingleCore::AND(arch::Regs aFirst, arch::Immediate aSecond) noexcept
{
    const arch::Immediate myResult = readRegister(aFirst) & aSecond;
    writeRegister(aFirst, myResult);
    AND_setFlags(myResult);
    return Trap::OK;
}

Trap SingleCore::AND(arch::MemoryAddress aFirst, arch::Immediate aSecond) noexcept
{
    const auto [myTrap, myMemoryValue] = theMemory.read(aFirst);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    const auto myResult = myMemoryValue & aSecond;
    const auto myWriteTrap = theMemory.write(aFirst, myResult);
    if (myWriteTrap != Trap::OK)
    {
        return myWriteTrap;
    }
    AND_setFlags(myResult);
    return Trap::OK;
}

Trap SingleCore::AND(arch::MemoryAddress aFirst, arch::Regs aSecond) noexcept
{
    const arch::Immediate mySecond = readRegister(aSecond);
    return AND(aFirst, mySecond);
}

Trap SingleCore::CALL(arch::MemoryAddress) noexcept
{
    return Trap::ILLEGAL;
}

Trap SingleCore::CALL(arch::Regs) noexcept
{
    return Trap::ILLEGAL;
}

Trap SingleCore::CBW(void) noexcept
{
    const bool myIsUpperBitSet = (theAX.theRegisterValue & 0x80) != 0;
    const auto myAH = myIsUpperBitSet ? 0xFF : 0x0;
    theAX.theRegisterValue = (myAH << constants::CHAR_SIZE) | theAX.theRegisterValue;
    return Trap::OK;
}

Trap SingleCore::CMP(arch::Regs aFirst, arch::Regs aSecond) noexcept
{
    return CMP(aFirst, readRegister(aSecond));
}

Trap SingleCore::CMP(arch::Regs aFirst, arch::MemoryAddress aSecond) noexcept
{
    const auto [myTrap, myMemoryValue] = theMemory.read(aSecond);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    return CMP(aFirst, myMemoryValue);
}

Trap SingleCore::CMP(arch::Regs aFirst, arch::Immediate aSecond) noexcept
{
    setFlagOnCmp(readRegister(aFirst), aSecond);
    return Trap::OK;
}

Trap SingleCore::CMP(arch::MemoryAddress aFirst, arch::Immediate aSecond) noexcept
{
    const auto [myTrap, myMemoryValue] = theMemory.read(aFirst);
    if (myTrap != Trap::OK)
    {
        return myTrap;
    }
    setFlagOnCmp(myMemoryValue, aSecond);
    return Trap::OK;
}

Trap SingleCore::CMP(arch::MemoryAddress aFirst, arch::Regs aSecond) noexcept
{
    return CMP(aFirst, static_cast<arch::Immediate>(readRegister(aSecond)));
}

template <SingleCore::BinaryOp Op>
arch::Immediate SingleCore::computeArithmeticFlags(std::uint32_t aDest, std::uint32_t aSource,
                                                   std::uint32_t aCarry) noexcept
{
    std::uint32_t myResult{};

    if constexpr (Op == SingleCore::BinaryOp::Add)
    {
        myResult = aDest + aSource + aCarry;

        setFlag(arch::Flags::CF, myResult > 0xFFFF);
        setFlag(arch::Flags::AF, ((aDest & 0xF) + (aSource & 0xF) + aCarry) > 0xF);
        setFlag(arch::Flags::OF, ((aDest ^ myResult) & (aSource ^ myResult) & 0x8000) != 0);
    }
    else if constexpr (Op == SingleCore::BinaryOp::Sub)
    {
        myResult = aDest - (aSource + aCarry);
        setFlag(arch::Flags::CF, aDest < (aSource + aCarry));
        setFlag(arch::Flags::AF, ((aDest ^ aSource ^ myResult) & 0x10) != 0);
        setFlag(arch::Flags::OF, ((aDest ^ aSource) & (aDest ^ myResult) & 0x8000) != 0);
    }

    myResult &= 0xFFFF;
    setFlag(arch::Flags::SF, (myResult & 0x8000) != 0);
    setFlag(arch::Flags::ZF, myResult == 0);
    setFlag(arch::Flags::PF, SingleCoreUtil::parity(myResult & 0xFF));

    return myResult;
}

arch::Immediate SingleCore::setFlagOnCmp(std::uint32_t aFirst, std::uint32_t aSecond) noexcept
{
    return computeArithmeticFlags<SingleCore::BinaryOp::Sub>(aFirst, aSecond, 0);
}

} // namespace svm
