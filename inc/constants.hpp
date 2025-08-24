#pragma once
#include <cstddef>

namespace svm
{
namespace constants
{
constexpr const std::size_t REGISTER_HALF_SIZE = 8U;
constexpr const std::size_t MAX_MEMORY_CAPACITY = 1 << 20;
constexpr const std::size_t REG_UPPER_HALF_MASK = 0xFF00U;
constexpr const std::size_t REG_LOWER_HALF_MASK = 0x00FFU;
constexpr const std::size_t MAX_REGISTER_VALUE = REG_UPPER_HALF_MASK | REG_LOWER_HALF_MASK;
constexpr const std::size_t SIGN_BIT_MASK = 0x8000U;
constexpr const std::size_t MEMORY_BUS_ALIGNMENT = 0x10U;
constexpr const std::size_t CHAR_SIZE = 8U;
constexpr const std::size_t BYTE_MASK = 0xFF;
} // namespace constants

} // namespace svm
