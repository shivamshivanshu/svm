#include "single_core.hpp"
#include "arch.hpp"
#include "memory.hpp"

#include <iostream>
using namespace std;
using namespace svm;
int main()
{
    auto ptr = make_shared<Memory>();
    SingleCore cpu(ptr);
    cpu.MOV_ri(Regs::AX, 0x0001);
    cpu.MOV_ri(Regs::BX, 0x0001);

    cpu.XOR_rr(Regs::AX, Regs::BX);
    cout << cpu.readRegister(Regs::AX) << endl;
    return 0;
}