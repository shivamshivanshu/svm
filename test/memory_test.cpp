#include "arch.hpp"
#include "memory.hpp"
#include "trap.hpp"

#include <gtest/gtest.h>

class RandomAccessMemoryTest : public ::testing::Test
{
protected:
    using Memory = svm::RandomAccessMemory;
    using MemoryAddr = svm::arch::MemoryAddress;
    using Immediate = svm::arch::Immediate;

    svm::RandomAccessMemory theMemory;
};

TEST_F(RandomAccessMemoryTest, WriteValidAddressMustBeCorrect)
{
    const auto myTrap = theMemory.write(svm::arch::MemoryAddress{.theAddress = 0xFFFF}, 0x15);
    EXPECT_TRUE(myTrap == svm::Trap::OK);
}

TEST_F(RandomAccessMemoryTest, ReadValidAddressMustBeCorrect)
{
    const auto myWriteTrap = theMemory.write(svm::arch::MemoryAddress{.theAddress = 0x1234}, 69);
    const auto [myReadTrap, myValue] = theMemory.read(svm::arch::MemoryAddress{.theAddress = 0x1234});
    EXPECT_TRUE(myWriteTrap == svm::Trap::OK);
    EXPECT_TRUE(myReadTrap == svm::Trap::OK);
    EXPECT_TRUE(myValue == 69);
}

TEST_F(RandomAccessMemoryTest, WriteInvalidAddressMustRaiseTrap)
{
    const auto myTrap = theMemory.write(svm::arch::MemoryAddress{.theAddress = 0xFFFFFFFF}, 0x15);
    EXPECT_TRUE(myTrap == svm::Trap::SEG_FAULT);
}

TEST_F(RandomAccessMemoryTest, ReadInvalidAddressMustRaiseTrap)
{
    const auto [myTrap, _] = theMemory.read(svm::arch::MemoryAddress{.theAddress = 0xFFFFFFFF});
    EXPECT_TRUE(myTrap == svm::Trap::SEG_FAULT);
}
