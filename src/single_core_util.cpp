
#include "single_core_util.hpp"
#include <cstdint>

namespace svm
{
bool SingleCoreUtil::parity(std::uint8_t aValue) noexcept
{
    bool myCurrParity = true;
    for (std::size_t i{}; i < 8; ++i)
    {
        if (aValue & (1 << i))
        {
            myCurrParity = !myCurrParity;
        }
    }
    return myCurrParity;
}
} // namespace svm
