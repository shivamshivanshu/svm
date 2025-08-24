#pragma once
#include <cstdint>

namespace svm::arch
{

// Registers
enum class Regs
{
    AX,  // Accumulator Register
    BX,  // Base Register
    CX,  // Count Register
    DX,  // Data Register
    CS,  // Code Segment
    DS,  // Data Segment
    SS,  // Stack Segment
    ES,  // Extra Segment
    SP,  // Stack Pointer
    BP,  // Base Pointer
    SI,  // Source Index
    DI,  // Destination Index
    IP,  // Instruction Pointer
    FLAG // Flag Register
};

enum class Flags
{
    CF = 0,  // Carry Flag
    PF = 2,  // Parity Flag
    AF = 4,  // Auxiliary Carry Flag
    ZF = 6,  // Zero Flag
    SF = 7,  // Sign Flag
    TF = 8,  // Trap Flag
    IF = 9,  // Interrupt Flag
    DF = 10, // Direction Flag
    OF = 11  // Overlflow Flag
};

// Basic Types

struct Register
{
    Regs theLabel;
    std::uint16_t theRegisterValue;
};

enum class RegLevel
{
    Low,
    High
};

using Immediate = std::uint16_t;

struct MemoryAddress
{
    std::uint32_t theAddress;
};

// CPU Instruction Set
enum class Inst
{
    AAA,   // ASCII Adjust after Addition. Corrects result in AH and AL after
           // addition when working with BCD values
    AAD,   // ASCII Adjust before Division Prepares two BCD values for division.
    AAM,   // ASCII Adjust after Multiplication. Corrects the result of
           // multiplication of two BCD values.
    AAS,   // ASCII Adjust after Subtraction. Corrects result in AH and AL after
           // subtraction when working with BCD values.
    ADC,   // Add with Carry.
    ADD,   // Add.
    AND,   // Logical AND between all bits of two operands. Result is stored in
           // operand1.
    CALL,  // Transfers control to procedure, return address is (IP) is pushed to
           // stack. 4-byte address may be entered in this form: 1234h:5678h, first
           // value is a segment second value is an offset (this is a far call, so
           // CS is also pushed to stack).
    CBW,   // Convert byte into word
    CLC,   // Clear Carry Flag
    CLD,   // Clear Direction Flag
    CLI,   // Clear Interrupt Flag
    CMC,   // Complement Carry Flag
    CMP,   // Compare and set flag
    CMPSB, // Compare bytes: ES:[DI] from DS:[SI]
    CMPSW, // Compare words: ES:[DI] from DS:[SI]
    CWD,   // Convert Word to Double word.
    DAA,   // Decimal adjust After Addition. Corrects the result of addition of two
           // packed BCD values.
    DAS,   // Decimal adjust After Substraction. Corrects the result of Substraction
           // of two packed BCD values.
    DEC,   // Decrement
    DIV,   // Unsigned divide.
    HLT,   // Halt the system
    IDIV,  // Signed divide
    IMUL,  // Signed Multiply
    IN,    // Input from port into AL or AX
    INC,   // Increment
    INT,   // Interrupt
    INTO,  // Interrupt 4 if Overflow flag is 1.
    IRET,  // Interrupt Return
    JA,    // Short Jump if first operand is Above second operand (as set by CMP
           // instruction). Unsigned.
    JAE,   // Short Jump if first operand is Above or Equal to second operand (as
           // set by CMP instruction). Unsigned
    JB,    // Short Jump if first operand is Below second operand (as set by CMP
           // instruction). Unsigned.
    JBE,   // Short Jump if first operand is Below or Equal to second operand (as
           // set by CMP instruction). Unsigned.
    JC,    // Short Jump if Carry flag is set to 1.
    JCXZ,  // Short Jump if CX register is 0.
    JE,    // Short Jump if first operand is Equal to second operand (as set by CMP
           // instruction). Signed/Unsigned.
    JG,    // Short Jump if first operand is Greater then second operand (as set by
           // CMP instruction). Signed
    JGE,   // Short Jump if first operand is Greater or Equal to second operand (as
           // set by CMP instruction). Signed.
    JL,    // Short Jump if first operand is Less then second operand (as set by CMP
           // instruction). Signed.
    JMP,   // Unconditional Jump. Transfers control to another part of the program.
           // 4-byte address may be entered in this form: 1234h:5678h, first value
           // is a segment second value is an offset.
    JLE,   // Short Jump if first operand is Less or Equal to second operand (as set
           // by CMP instruction). Signed.
    JNA,   // Short Jump if first operand is Not Above second operand (as set by CMP
           // instruction). Unsigned.
    JNAE,  // Short Jump if first operand is Not Above and Not Equal to second
           // operand (as set by CMP instruction). Unsigned.
    JNB,   // Short Jump if first operand is Not Below second operand (as set by CMP
           // instruction). Unsigned.
    JNBE,  // Short Jump if first operand is Not Below and Not Equal to second
           // operand (as set by CMP instruction). Unsigned.
    JNC,   // Short Jump if Carry flag is set to 0.
    JNE,   // Short Jump if first operand is Not Equal to second operand (as set by
           // CMP instruction). Signed/Unsigned.
    JNG,   // Short Jump if first operand is Not Greater then second operand (as set
           // by CMP instruction). Signed.
    JNGE,  // Short Jump if first operand is Not Greater and Not Equal to second
           // operand (as set by CMP instruction). Signed.
    JNL,   // Short Jump if first operand is Not Less then second operand (as set by
           // CMP instruction). Signed.
    JNLE,  // Short Jump if first operand is Not Less and Not Equal to second
           // operand (as set by CMP instruction). Signed.
    JNO,   // Short Jump if Not Overflow.
    JNP,   // Short Jump if No Parity (odd). Only 8 low bits of result are checked.
           // Set by CMP, SUB, ADD, TEST, AND, OR, XOR instructions.
    JNS,   // Short Jump if Not Signed (if positive). Set by CMP, SUB, ADD, TEST,
           // AND, OR, XOR instructions.
    JNZ,   // Short Jump if Not Zero (not equal). Set by CMP, SUB, ADD, TEST, AND,
           // OR, XOR instructions.
    JO,    // Short Jump if Overflow.
    JP,    // Short Jump if Parity (even). Only 8 low bits of result are checked. Set
        // by CMP, SUB, ADD, TEST, AND, OR, XOR instructions.
    JPE,    // Short Jump if Parity Even. Only 8 low bits of result are checked. Set
            // by CMP, SUB, ADD, TEST, AND, OR, XOR instructions.
    JPO,    // Short Jump if Parity Odd. Only 8 low bits of result are checked. Set
            // by CMP, SUB, ADD, TEST, AND, OR, XOR instructions.
    JS,     // Short Jump if Signed (if negative). Set by CMP, SUB, ADD, TEST, AND,
            // OR, XOR instructions.
    JZ,     // Short Jump if Zero (equal). Set by CMP, SUB, ADD, TEST, AND, OR, XOR
            // instructions.
    LAHF,   // Load AH from 8 low bits of Flags register.
    LDS,    // Load memory double word into word register and DS.
    LEA,    // Load Effective Address.
    LES,    // Load memory double word into word register and ES.
    LODSB,  // Load byte at DS:[SI] into AL. Update SI.
    LODSW,  // Load word at DS:[SI] into AX. Update SI
    LOOP,   // Decrease CX, jump to label if CX not zero.
    LOOPE,  // Decrease CX, jump to label if CX not zero and Equal (ZF = 1).
    LOOPNE, // Decrease CX, jump to label if CX not zero and Not Equal (ZF = 0).
    LOOPNZ, // Decrease CX, jump to label if CX not zero and ZF = 0.
    LOOPZ,  // Decrease CX, jump to label if CX not zero and ZF = 1.
    MOV,    // Copy operand2 to operand1.
    MOVSB,  // Copy byte at DS:[SI] to ES:[DI]. Update SI and DI.
    MOVSW,  // Copy word at DS:[SI] to ES:[DI]. Update SI and DI.
    MUL,    // Unsigned multiply.
    NEG,    // Negate. Makes operand negative (two's complement).
    NOP,    // No Operation
    NOT,    // Invert each bit of the operand.
    OR,     // Logical OR between all bits of two operands. Result is stored in first
            // operand.
    OUT,    // Output from AL or AX to port. First operand is a port number. If
            // required to access port number over 255 - DX register should be used.
    POP,    // Get 16 bit value from the stack.
    POPA,   // Pop all general purpose registers DI, SI, BP, SP, BX, DX, CX, AX from
            // the stack. SP value is ignored, it is Popped but not set to SP
            // register).
    POPF,   // Get flags register from the stack.
    PUSH,   // Store 16 bit value in the stack. Note: PUSH immediate works only on
            // 80186 CPU and later!
    PUSHA,  // Push all general purpose registers AX, CX, DX, BX, SP, BP, SI, DI in
            // the stack. Original value of SP register (before PUSHA) is used.
    PUSHF,  // Store flags register in the stack.
    RCL,    // Rotate operand1 left through Carry Flag. The number of rotates is set
            // by operand2.
    RCR,    // Rotate operand1 right through Carry Flag. The number of rotates is set
            // by operand2.
    REP,    // Repeat following MOVSB, MOVSW, LODSB, LODSW, STOSB, STOSW instructions
            // CX times.
    REPE,   // Repeat following CMPSB, CMPSW, SCASB, SCASW instructions while ZF = 1
            // (result is Equal), maximum CX times.
    REPNE,  // Repeat following CMPSB, CMPSW, SCASB, SCASW instructions while ZF =
            // 0 (result is Not Equal), maximum CX times.
    REPNZ,  // Repeat following CMPSB, CMPSW, SCASB, SCASW instructions while ZF =
            // 0 (result is Not Zero), maximum CX times.
    REPZ,   // Repeat following CMPSB, CMPSW, SCASB, SCASW instructions while ZF = 1
            // (result is Zero), maximum CX times.
    RET,    // Return from near procedure.
    RETF,   // Return from Far procedure.
    ROL,    // Rotate operand1 left. The number of rotates is set by operand2
    ROR,    // Rotate operand1 right. The number of rotates is set by operand2.
    SAHF,   // Store AH register into low 8 bits of Flags register.
    SAL,    // Shift Arithmetic operand1 Left. The number of shifts is set by
            // operand2.
    SAR,    // Shift Arithmetic operand1 Right. The number of shifts is set by
            // operand2.
    SBB,    // Subtract with Borrow.
    SCASB,  // Compare bytes: AL from ES:[DI].
    SCASW,  // Compare words: AX from ES:[DI].
    SHL,    // Shift operand1 Left. The number of shifts is set by operand2
    SHR,    // Shift operand1 Right. The number of shifts is set by operand2
    STC,    // Set Carry flag
    STD,    // Set Direction flag. SI and DI will be decremented by chain
            // instructions: CMPSB, CMPSW, LODSB, LODSW, MOVSB, MOVSW, STOSB, STOSW.
    STI,    // Set Interrupt enable flag. This enables hardware interrupts.
    STOSB,  // Store byte in AL into ES:[DI]. Update SI.
    STOSW,  // Store word in AX into ES:[DI]. Update SI.
    SUB,    // Subtract.
    TEST,   // Logical AND between all bits of two operands for flags only. These
            // flags are effected: ZF, SF, PF. Result is not stored anywhere.
    XCHG,   // Exchange values of two operands.
    XLATB,  // Translate byte from table. Copy value of memory byte at DS:[BX +
            // unsigned AL] to AL register.
    XOR,    // Logical XOR (Exclusive OR) between all bits of two operands. Result is
            // stored in first operand.
};

enum class Operand
{
    Register,
    Immediate,
    MemoryAddress
};

} // namespace svm::arch
