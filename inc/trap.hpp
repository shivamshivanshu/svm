#pragma once
namespace svm
{

    enum class Trap
    {
        OK,
        ILLEGAL,
        SEG_FAULT,
        MEM_FAULT,
        HALT,
    };

} // namespace svm
