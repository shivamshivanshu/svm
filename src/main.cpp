#include "single_core.hpp"
#include "instructions.hpp"
#include "memory.hpp"

#include <iostream>
using namespace std;
using namespace svm;
int main()
{
    auto ptr = make_shared<Memory<uint16_t, 1024>>();
    SingleCore cpu(ptr);
    cpu.MOV_ri(Regs::AX, 6);
    cout << static_cast<int>(cpu.readRegister(Regs::AX)) << std::endl;
    cpu.MOV_ri(Regs::BX, 2);
    cout << static_cast<int>(cpu.readRegister(Regs::BX)) << std::endl;
    cpu.AND_rr(Regs::AX, Regs::BX);
    cout << static_cast<int>(cpu.readRegister(Regs::AX)) << std::endl;
    return 0;
}