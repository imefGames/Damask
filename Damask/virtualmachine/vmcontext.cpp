#include <virtualmachine/vmcontext.h>

#include <algorithm>
#include <string_view>

namespace VirtualMachine
{
    VMContext::VMContext()
        : m_CurrentInstruction{ 0 }
        , m_Flags{ 0 }
    {
        std::fill(std::begin(m_GeneralPurposeRegisters), std::end(m_GeneralPurposeRegisters), 0);

        m_RawMemorySize = (1 << 14);
        m_RawMemory = static_cast<char*>(std::malloc(m_RawMemorySize));

        m_Instructions[static_cast<size_t>(Instruction::Nop)] = nullptr;
        m_Instructions[static_cast<size_t>(Instruction::Call)] = Instructions::Call;
        m_Instructions[static_cast<size_t>(Instruction::CallNative)] = Instructions::CallNative;
        m_Instructions[static_cast<size_t>(Instruction::Ret)] = Instructions::Return;
        m_Instructions[static_cast<size_t>(Instruction::Push)] = Instructions::Push;
        m_Instructions[static_cast<size_t>(Instruction::Pop)] = Instructions::Pop;

        m_Instructions[static_cast<size_t>(Instruction::Mov)] = Instructions::Move;
        m_Instructions[static_cast<size_t>(Instruction::Cmp)] = Instructions::Compare;

        m_Instructions[static_cast<size_t>(Instruction::Jmp)] = Instructions::Jump;
        m_Instructions[static_cast<size_t>(Instruction::Je)] = Instructions::JumpEqual;
        m_Instructions[static_cast<size_t>(Instruction::Jne)] = Instructions::JumpNotEqual;
        m_Instructions[static_cast<size_t>(Instruction::Jl)] = Instructions::JumpLower;
        m_Instructions[static_cast<size_t>(Instruction::Jle)] = Instructions::JumpLowerEqual;
        m_Instructions[static_cast<size_t>(Instruction::Jg)] = Instructions::JumpGreater;
        m_Instructions[static_cast<size_t>(Instruction::Jge)] = Instructions::JumpGreaterEqual;

        m_Instructions[static_cast<size_t>(Instruction::Inc)] = Instructions::Increment;
        m_Instructions[static_cast<size_t>(Instruction::Dec)] = Instructions::Decrement;
        m_Instructions[static_cast<size_t>(Instruction::Add)] = Instructions::Add;
        m_Instructions[static_cast<size_t>(Instruction::Sub)] = Instructions::Substract;
        m_Instructions[static_cast<size_t>(Instruction::Mul)] = Instructions::Multiply;
        m_Instructions[static_cast<size_t>(Instruction::Div)] = Instructions::Divide;
        m_Instructions[static_cast<size_t>(Instruction::Mod)] = Instructions::Modulo;
        m_Instructions[static_cast<size_t>(Instruction::Shl)] = Instructions::ShiftLeft;
        m_Instructions[static_cast<size_t>(Instruction::Shr)] = Instructions::ShiftRight;


        /*
        TODO:
            Organise the virtual memory segments
                - text: executable code
                - data: initialized static variables
                - bss: uninitialized static variables
                - heap: for heap allocation
                - stack: for stack allocation (at the end on the virtual memory)
        */
    }

    void VMContext::RunInstructions()
    {
        while (false) //TODO Have end condition
        {
            Instruction currentInstruction{ ReadFromInstructionBuffer<Instruction>() };
            if (InstructionCallback instructionCallback = GetInstruction(currentInstruction))
            {
                instructionCallback(*this);
            }
        }
    }

    void VMContext::PushToStack(long long value)
    {
        long long& stackPointerRegister{ m_GeneralPurposeRegisters[static_cast<size_t>(MemoryLocation::RegisterSP) - static_cast<size_t>(MemoryLocation::RegisterFirst)] };
        long long* memoryLocation{ reinterpret_cast<long long*>(&m_RawMemory[stackPointerRegister]) };
        *memoryLocation = value;
        stackPointerRegister -= sizeof(long long);
    }

    long long VMContext::PopFromStack()
    {
        long long& stackPointerRegister{ m_GeneralPurposeRegisters[static_cast<size_t>(MemoryLocation::RegisterSP) - static_cast<size_t>(MemoryLocation::RegisterFirst)] };
        long long* memoryLocation{ reinterpret_cast<long long*>(&m_RawMemory[stackPointerRegister]) };
        stackPointerRegister += sizeof(long long);
        return *memoryLocation;
    }

    VMContext::InstructionCallback VMContext::GetInstruction(Instruction instruction) const
    {
        size_t instructionIndex{ static_cast<size_t>(instruction) };
        InstructionCallback foundInstruction{ nullptr };
        if (instructionIndex < static_cast<size_t>(Instruction::Count))
        {
            foundInstruction = m_Instructions[instructionIndex];
        }
        return foundInstruction;
    }

    void* VMContext::GetMemoryLocationPointerAtCurrentInstructionInternal(size_t elementSize)
    {
        MemoryLocation location{ ReadFromInstructionBuffer<MemoryLocation>() };
        if (location >= MemoryLocation::RegisterFirst && location <= MemoryLocation::RegisterLast)
        {
            return &m_GeneralPurposeRegisters[static_cast<size_t>(location) - static_cast<size_t>(MemoryLocation::RegisterFirst)];
        }
        else if (location == MemoryLocation::Constant)
        {
            void* constantLocation{ &m_RawMemory[m_CurrentInstruction] };
            m_CurrentInstruction += elementSize;
            return constantLocation;
        }
        else if (location == MemoryLocation::MemoryAdress)
        {
            size_t memoryLocation{ ReadFromInstructionBuffer<size_t>() };
            if (memoryLocation < m_RawMemorySize)
            {
                return &m_RawMemory[memoryLocation];
            }
            else
            {
                //TODO: error
            }
        }
        else
        {
            //TODO: error
        }
    }
}