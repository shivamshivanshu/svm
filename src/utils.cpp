#include "single_core.hpp"

namespace svm
{
	Trap SingleCore::CLD() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = ~(static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::DF));
		lvalue &= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::CLI() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = ~(static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::IF));
		lvalue &= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::CLC() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = ~(static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::CF));
		lvalue &= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::CMC() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = (static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::CF));
		Immediate cfValue = lvalue & mask;
		if (cfValue)
		{
			mask = ~mask;
			lvalue &= mask;
		}
		else
		{
			lvalue |= mask;
		}
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::NOP() noexcept
	{
		return Trap::OK;
	}
	Trap SingleCore::HLT() noexcept
	{
		return Trap::HALT;
	}
	Trap SingleCore::STC() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = (static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::CF));
		lvalue |= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::STD() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = (static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::DF));
		lvalue |= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
	Trap SingleCore::STI() noexcept
	{
		auto lvalue = readRegister(Regs::FLAG);
		Immediate mask = (static_cast<Immediate>(1U) << static_cast<std::size_t>(Flags::IF));
		lvalue |= mask;
		writeRegister(Regs::FLAG, lvalue);
		return Trap::OK;
	}
} // namespace svm
