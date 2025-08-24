#include <type_traits>
#include <unistd.h>
#include <utility>

#include "arch.hpp"
#include "single_core.hpp"
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
    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xFF);
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

    arch::Immediate myAx = ((myAH & 0xFF) << 8) | (myAL & 0xFF);
    theAX.theRegisterValue = myAx;
    return Trap::OK;
}

arch::Immediate SingleCore::setFlagOnAdd(std::uint32_t aSource, std::uint32_t aDest, std::uint32_t aCarry) noexcept
{
    auto myParity = [](std::uint8_t aValue) -> bool {
        bool myCurrParity = true;
        for (std::size_t i{}; i < 8; ++i)
        {
            if (aValue & (1 << i))
            {
                myCurrParity = !myCurrParity;
            }
        }
        return myCurrParity;
    };

    const auto myResult = aSource + aDest + aCarry;

    const bool myIsCarryFlag = myResult > 0xFFFF;
    const bool myAuxiliaryFlag = ((aSource & 0xF) + (aDest & 0xF) + aCarry) > 0xF;
    const bool myIsOverflowFlag = ((aDest ^ myResult) & (aSource ^ myResult) & 0x8000) != 0;
    const bool myIsSignFlag = (myResult & 0x8000) != 0;
    const bool myIsZeroFlag = (myResult & 0xFFFF) == 0;
    const bool myIsParityFlag = myParity(myResult & 0xFF);

    setFlag(arch::Flags::CF, myIsCarryFlag);
    setFlag(arch::Flags::AF, myAuxiliaryFlag);
    setFlag(arch::Flags::OF, myIsOverflowFlag);
    setFlag(arch::Flags::SF, myIsSignFlag);
    setFlag(arch::Flags::ZF, myIsZeroFlag);
    setFlag(arch::Flags::PF, myIsParityFlag);

    return myResult & 0xFFFF;
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

} // namespace svm
