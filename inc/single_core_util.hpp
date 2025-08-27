#pragma once

#include <cstdint>

namespace svm
{
struct SingleCoreUtil
{
    static bool parity(std::uint8_t) noexcept;
};
}
