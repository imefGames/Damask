#include <virtualmachine/instructions.h>

#include <virtualmachine/vmcontext.h>

namespace VirtualMachine
{
    namespace Instructions
    {
        void Call(VMContext& context)
        {
            size_t functionFirstInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            size_t returnInstruction{ context.GetCurrentInstruction() + 1 };
            context.PushToStack(returnInstruction);
            context.JumpToInstruction(functionFirstInstruction);
        }

        void CallNative(VMContext& context)
        {
            //TODO
        }

        void Return(VMContext& context)
        {
            //TODO: if this is the return for main, terminate.
            size_t returnInstruction{ static_cast<size_t>(context.PopFromStack()) };
            context.JumpToInstruction(returnInstruction);
        }

        void Push(VMContext& context)
        {
            long long* memoryLocation = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            context.PushToStack(*memoryLocation);
        }

        void Pop(VMContext& context)
        {
            long long* memoryLocation = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *memoryLocation = context.PopFromStack();
        }

        void Move(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination = *source;
        }

        void Compare(VMContext& context)
        {
            long long* lhs = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* rhs = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            
            long long comparison{ *lhs - *rhs };
            char flags{ 0x0 };
            if (comparison == 0) { flags |= static_cast<char>(Flag::ZeroFlag); }
            if (comparison < 0) { flags |= static_cast<char>(Flag::SignFlag); }
            context.SetFlags(flags);
        }

        void Jump(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            context.JumpToInstruction(destinationInstruction);
        }

        void JumpEqual(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (context.IsFlagRaised(Flag::ZeroFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void JumpNotEqual(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (!context.IsFlagRaised(Flag::ZeroFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void JumpLower(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (!context.IsFlagRaised(Flag::ZeroFlag) && context.IsFlagRaised(Flag::SignFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void JumpLowerEqual(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (context.IsFlagRaised(Flag::ZeroFlag) && context.IsFlagRaised(Flag::SignFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void JumpGreater(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (!context.IsFlagRaised(Flag::ZeroFlag) && !context.IsFlagRaised(Flag::SignFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void JumpGreaterEqual(VMContext& context)
        {
            size_t destinationInstruction{ context.ReadFromInstructionBuffer<size_t>() };
            if (context.IsFlagRaised(Flag::ZeroFlag) && !context.IsFlagRaised(Flag::SignFlag))
            {
                context.JumpToInstruction(destinationInstruction);
            }
        }

        void Increment(VMContext& context)
        {
            long long* memoryLocation = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            ++(*memoryLocation);
        }

        void Decrement(VMContext& context)
        {
            long long* memoryLocation = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            --(*memoryLocation);
        }

        void Add(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination += *source;
        }

        void Substract(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination -= *source;
        }

        void Multiply(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination *= *source;
        }

        void Divide(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination /= *source;
        }

        void Modulo(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination %= *source;
        }

        void ShiftLeft(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination <<= *source;
        }

        void ShiftRight(VMContext& context)
        {
            long long* destination = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            long long* source = context.GetMemoryLocationPointerAtCurrentInstruction<long long>();
            *destination >>= *source;
        }
    }
}