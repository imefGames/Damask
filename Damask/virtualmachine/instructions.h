#pragma once

namespace VirtualMachine
{
    class VMContext;

    //TODO: Allow instructions for bytes, Words, DWords, & QWords
    enum class Instruction : char
    {
        Nop,
        Call,
        CallNative,
        Ret,
        Push,
        Pop,

        Mov,
        Cmp,

        Jmp,
        Je,
        Jne,
        Jl,
        Jle,
        Jg,
        Jge,

        Inc,
        Dec,
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Shl,
        Shr,

        Count
    };

    enum class MemoryLocation : char
    {
        RegisterA,
        RegisterB,
        RegisterC,
        RegisterD,
        RegisterSI,
        RegisterDI,
        RegisterBP,
        RegisterSP,
        Register8,
        Register9,
        Register10,
        Register11,
        Register12,
        Register13,
        Register14,
        Register15,

        Constant,
        MemoryAdress, //TODO: have versions with offset and for arrays

        RegisterFirst = RegisterA,
        RegisterLast = Register15,
    };

    enum class Flag : char
    {
        ZeroFlag = 0x01,
        SignFlag = 0x02
    };

    namespace Instructions
    {
        void Call(VMContext& context);
        void CallNative(VMContext& context);
        void Return(VMContext& context);
        void Push(VMContext& context);
        void Pop(VMContext& context);

        void Move(VMContext& context);
        void Compare(VMContext& context);

        void Jump(VMContext& context);
        void JumpEqual(VMContext& context);
        void JumpNotEqual(VMContext& context);
        void JumpLower(VMContext& context);
        void JumpLowerEqual(VMContext& context);
        void JumpGreater(VMContext& context);
        void JumpGreaterEqual(VMContext& context);

        void Increment(VMContext& context);
        void Decrement(VMContext& context);
        void Add(VMContext& context);
        void Substract(VMContext& context);
        void Multiply(VMContext& context);
        void Divide(VMContext& context);
        void Modulo(VMContext& context);
        void ShiftLeft(VMContext& context);
        void ShiftRight(VMContext& context);
    }
}