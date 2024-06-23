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
    cpu.mem->write(0, 5);
    cpu.POP_r(Regs::AX);
    cout << cpu.readRegister(Regs::AX) << endl;
    cout << cpu.readRegister(Regs::SP) << endl;
    return 0;
}