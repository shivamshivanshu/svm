#include "arch.hpp"
#include "memory.hpp"
#include "single_core.hpp"
#include "trap.hpp"

#include <gtest/gtest.h>

class SingleCoreTest : public ::testing::Test
{

  protected:
    svm::RandomAccessMemory theMemory;
    svm::SingleCore theCpu;

  public:
    using Regs = svm::arch::Regs;
    using Immediate = svm::arch::Immediate;
    using MemoryAddr = svm::arch::MemoryAddress;
    using Flags = svm::arch::Flags;
    using Trap = svm::Trap;

    static constexpr auto DEFAULT_WRITE_VALUE = 42;
    static constexpr auto DEFAULT_IO_REG = svm::arch::Regs::AX;
    static constexpr auto DEFAULT_MEM_ADDR = MemoryAddr{.theAddress = 0x1234};

    SingleCoreTest() : theMemory{}, theCpu{theMemory}
    {
        theCpu.getReg(DEFAULT_IO_REG).theRegisterValue = 42;
    }
};

TEST_F(SingleCoreTest, RegisterShouldWriteCorrectValue)
{
    theCpu.writeRegister(Regs::BX, Immediate{69});
    EXPECT_TRUE(theCpu.getReg(Regs::BX).theRegisterValue == 69);
}

TEST_F(SingleCoreTest, RegisterShouldReadCorrectValue)
{
    const auto myValue = theCpu.readRegister(DEFAULT_IO_REG);
    EXPECT_TRUE(myValue == 42);
}

TEST_F(SingleCoreTest, AAA_Test)
{
    theCpu.writeRegister(Regs::AX, 15);
    theCpu.AAA();

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x5);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x1);
}

TEST_F(SingleCoreTest, AAD_Test)
{
    theCpu.writeRegister(Regs::AX, 0x105);
    theCpu.AAD();

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0xF);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x0);
}

TEST_F(SingleCoreTest, AAM_Test)
{
    theCpu.writeRegister(Regs::AX, 0xF);
    theCpu.AAM();

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x5);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x1);
}

TEST_F(SingleCoreTest, AAS_Test)
{
    theCpu.writeRegister(Regs::AX, 0x2FF);
    theCpu.AAS();

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x9);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x1);
}

TEST_F(SingleCoreTest, Flag_Test)
{
    theCpu.STC();
    theCpu.STD();
    theCpu.STI();
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 1);
    EXPECT_EQ(theCpu.readFlag(Flags::DF), 1);
    EXPECT_EQ(theCpu.readFlag(Flags::IF), 1);

    theCpu.CLC();
    theCpu.CLD();
    theCpu.CLI();
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
    EXPECT_EQ(theCpu.readFlag(Flags::DF), 0);
    EXPECT_EQ(theCpu.readFlag(Flags::IF), 0);

    theCpu.setFlag(Flags::CF, 1);
    theCpu.CMC();
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_Reg_Mem_Test)
{
    theCpu.writeRegister(Regs::AX, 0x5);
    const auto myTrap = theMemory.write(DEFAULT_MEM_ADDR, 1);
    EXPECT_EQ(myTrap, svm::Trap::OK);
    theCpu.STC();
    theCpu.ADC(Regs::AX, DEFAULT_MEM_ADDR);

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x7);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x0);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_Reg_Reg_Test)
{
    theCpu.writeRegister(Regs::AX, 0x5);
    theCpu.writeRegister(Regs::BX, 0x1);
    theCpu.STC();
    theCpu.ADC(Regs::AX, Regs::BX);

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x7);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x0);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_Reg_Imm_Test)
{
    theCpu.writeRegister(Regs::AX, 0x5);
    theCpu.STC();
    theCpu.ADC(Regs::AX, 1);

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ((myAx & 0xFF), 0x7);
    EXPECT_EQ(((myAx >> 8) & 0xFF), 0x0);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_Mem_Imm_Test)
{
    const auto myWriteTrap = theMemory.write(DEFAULT_MEM_ADDR, 5);
    EXPECT_EQ(myWriteTrap, svm::Trap::OK);
    theCpu.STC();
    theCpu.ADC(DEFAULT_MEM_ADDR, 1);

    const auto [myReadTrap, myValue] = theMemory.read(DEFAULT_MEM_ADDR);
    EXPECT_EQ(myReadTrap, Trap::OK);
    EXPECT_EQ((myValue & 0xFF), 0x7);
    EXPECT_EQ(((myValue >> 8) & 0xFF), 0x0);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_Mem_Reg_Test)
{
    theCpu.writeRegister(Regs::AX, 0x5);
    const auto myTrap = theMemory.write(DEFAULT_MEM_ADDR, 1);
    EXPECT_EQ(myTrap, svm::Trap::OK);
    theCpu.STC();
    theCpu.ADC(DEFAULT_MEM_ADDR, Regs::AX);

    const auto [myReadTrap, myValue] = theMemory.read(DEFAULT_MEM_ADDR);
    EXPECT_EQ(myReadTrap, Trap::OK);
    EXPECT_EQ((myValue & 0xFF), 0x7);
    EXPECT_EQ(((myValue >> 8) & 0xFF), 0x0);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
}

TEST_F(SingleCoreTest, ADC_CF_Test)
{
    theCpu.writeRegister(Regs::AX, 0xFFFF);
    const auto myTrap = theMemory.write(DEFAULT_MEM_ADDR, 1);
    EXPECT_EQ(myTrap, svm::Trap::OK);
    theCpu.STC();
    theCpu.ADC(Regs::AX, DEFAULT_MEM_ADDR);

    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x1);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0x1);
}

TEST_F(SingleCoreTest, ADC_AuxiliaryFlag_Set)
{
    const auto myMemoryAddr = DEFAULT_MEM_ADDR;

    theCpu.writeRegister(Regs::AX, 0x000F);
    const auto myTrap = theMemory.write(myMemoryAddr, 0x0001);
    EXPECT_EQ(myTrap, Trap::OK);

    theCpu.STC(); // CF = 1
    theCpu.ADC(Regs::AX, myMemoryAddr);

    EXPECT_EQ(theCpu.readFlag(Flags::AF), 1);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0011);
}

TEST_F(SingleCoreTest, ADC_OverflowFlag_Set)
{
    const auto myMemoryAddr = DEFAULT_MEM_ADDR;

    theCpu.writeRegister(Regs::AX, 0x7FFF);
    const auto myTrap = theMemory.write(myMemoryAddr, 0x0001);
    EXPECT_EQ(myTrap, Trap::OK);

    theCpu.STC(); // CF = 1
    theCpu.ADC(Regs::AX, myMemoryAddr);

    EXPECT_EQ(theCpu.readFlag(Flags::OF), 1);
    EXPECT_EQ(theCpu.readFlag(Flags::SF), 1);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x8001);
}

TEST_F(SingleCoreTest, ADC_ZeroFlag_Set)
{
    const auto myMemoryAddr = DEFAULT_MEM_ADDR;

    theCpu.writeRegister(Regs::AX, 0xFFFF);
    const auto myTrap = theMemory.write(myMemoryAddr, 0x0000);
    EXPECT_EQ(myTrap, Trap::OK);

    theCpu.STC(); // CF = 1
    theCpu.ADC(Regs::AX, myMemoryAddr);

    EXPECT_EQ(theCpu.readFlag(Flags::ZF), 1);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0000);
}

TEST_F(SingleCoreTest, ADC_ParityFlag_Check)
{
    const auto myMemoryAddr = DEFAULT_MEM_ADDR;

    // Case 1: Result AL = 0x02 → PF = false
    theCpu.writeRegister(Regs::AX, 0x0001);
    auto myTrap = theMemory.write(myMemoryAddr, 0x0001);
    EXPECT_EQ(myTrap, Trap::OK);
    theCpu.CLC(); // CF = 0
    theCpu.ADC(Regs::AX, myMemoryAddr);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0002);
    EXPECT_EQ(theCpu.readFlag(Flags::PF), false);

    // Case 2: Result AL = 0x03 → PF = true
    theCpu.writeRegister(Regs::AX, 0x0002);
    myTrap = theMemory.write(myMemoryAddr, 0x0001);
    EXPECT_EQ(myTrap, Trap::OK);
    theCpu.CLC();
    theCpu.ADC(Regs::AX, myMemoryAddr);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0003);
    EXPECT_EQ(theCpu.readFlag(Flags::PF), true);
}

TEST_F(SingleCoreTest, AND_Reg_Reg_Test)
{
    theCpu.writeRegister(Regs::AX, 0xFFFF);
    theCpu.writeRegister(Regs::BX, 0x00FF);

    theCpu.AND(Regs::AX, Regs::BX);

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ(myAx, 0x00FF);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
    EXPECT_EQ(theCpu.readFlag(Flags::OF), 0);
}

TEST_F(SingleCoreTest, AND_Reg_Imm_Test)
{
    theCpu.writeRegister(Regs::AX, 0x0F0F);
    theCpu.AND(Regs::AX, 0x00FF);

    const auto myAx = theCpu.readRegister(Regs::AX);
    EXPECT_EQ(myAx, 0x000F);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
    EXPECT_EQ(theCpu.readFlag(Flags::OF), 0);
}

TEST_F(SingleCoreTest, AND_Mem_Imm_Test)
{
    const auto myWriteTrap = theMemory.write(DEFAULT_MEM_ADDR, 0xFFFF);
    EXPECT_EQ(myWriteTrap, svm::Trap::OK);

    theCpu.AND(DEFAULT_MEM_ADDR, 0x0F0F);

    const auto [myReadTrap, myValue] = theMemory.read(DEFAULT_MEM_ADDR);
    EXPECT_EQ(myReadTrap, Trap::OK);
    EXPECT_EQ(myValue, 0x0F0F);
    EXPECT_EQ(theCpu.readFlag(Flags::CF), 0);
    EXPECT_EQ(theCpu.readFlag(Flags::OF), 0);
}

TEST_F(SingleCoreTest, AND_Mem_Reg_Test)
{
    const auto myTrap = theMemory.write(DEFAULT_MEM_ADDR, 0xFFFF);
    EXPECT_EQ(myTrap, svm::Trap::OK);

    theCpu.writeRegister(Regs::BX, 0x00FF);
    theCpu.AND(DEFAULT_MEM_ADDR, Regs::BX);

    const auto [myReadTrap, myValue] = theMemory.read(DEFAULT_MEM_ADDR);
    EXPECT_EQ(myReadTrap, Trap::OK);
    EXPECT_EQ(myValue, 0x00FF);
}

TEST_F(SingleCoreTest, AND_SignFlag_Set)
{
    theCpu.writeRegister(Regs::AX, 0xFFFF);
    theCpu.AND(Regs::AX, 0x8000);

    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x8000);
    EXPECT_EQ(theCpu.readFlag(Flags::SF), 1);
}

TEST_F(SingleCoreTest, AND_ZeroFlag_Set)
{
    theCpu.writeRegister(Regs::AX, 0x0F0F);
    theCpu.AND(Regs::AX, 0xF0F0);

    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0000);
    EXPECT_EQ(theCpu.readFlag(Flags::ZF), 1);
}

TEST_F(SingleCoreTest, AND_ParityFlag_Check)
{
    const auto myMemoryAddr = DEFAULT_MEM_ADDR;

    // Case 1: AL = 0x02 → PF = false (odd parity)
    theCpu.writeRegister(Regs::AX, 0x0003);
    auto myTrap = theMemory.write(myMemoryAddr, 0x0002);
    EXPECT_EQ(myTrap, Trap::OK);
    theCpu.AND(Regs::AX, myMemoryAddr);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0002);
    EXPECT_EQ(theCpu.readFlag(Flags::PF), false);

    // Case 2: AL = 0x03 → PF = true (even parity)
    theCpu.writeRegister(Regs::AX, 0x0003);
    myTrap = theMemory.write(myMemoryAddr, 0x0003);
    EXPECT_EQ(myTrap, Trap::OK);
    theCpu.AND(Regs::AX, myMemoryAddr);
    EXPECT_EQ(theCpu.readRegister(Regs::AX), 0x0003);
    EXPECT_EQ(theCpu.readFlag(Flags::PF), true);
}
