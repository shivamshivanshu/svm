#include "arch.hpp"
#include "memory.hpp"
#include "single_core.hpp"

#include <gtest/gtest.h>

class SingleCoreTest : public ::testing::Test
{

  protected:
    svm::RandomAccessMemory theMemory;
    svm::SingleCore theCpu;

  public:
    static constexpr auto DEFAULT_WRITE_VALUE = 42;
    static constexpr auto DEFAULT_IO_REG = svm::arch::Regs::AX;

    SingleCoreTest() : theMemory{}, theCpu{theMemory}
    {
        theCpu.getReg(DEFAULT_IO_REG).theRegisterValue = 42;
    }
};

TEST_F(SingleCoreTest, RegisterShouldWriteCorrectValue)
{
    theCpu.writeRegister(svm::arch::Regs::BX, svm::arch::Immediate{69});
    EXPECT_TRUE(theCpu.getReg(svm::arch::Regs::BX).theRegisterValue == 69);
}

TEST_F(SingleCoreTest, RegisterShouldReadCorrectValue)
{
    const auto myValue = theCpu.readRegister(DEFAULT_IO_REG);
    EXPECT_TRUE(myValue == 42);
}

