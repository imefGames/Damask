#pragma once

#include <virtualmachine/instructions.h>

namespace VirtualMachine
{
    class VMContext
    {
    public:
        VMContext();
        void RunInstructions();

        template <class T>
        T* GetMemoryLocationPointerAtCurrentInstruction()
        {
            return reinterpret_cast<T*>(GetMemoryLocationPointerAtCurrentInstructionInternal(sizeof(T)));
        }

        template<class T>
        T ReadFromInstructionBuffer()
        {
            T readElement{};
            if (m_CurrentInstruction + sizeof(T) < m_RawMemorySize)
            {
                char* rawBuffer{ reinterpret_cast<char*>(&readElement) };
                for (size_t i = 0; i < sizeof(T); ++i)
                {
                    rawBuffer[i] = m_RawMemory[m_CurrentInstruction + i];
                }
                m_CurrentInstruction += sizeof(T);
            }
            else
            {
                //TODO: error
            }
            return readElement;
        }

        void PushToStack(long long value);
        long long PopFromStack();

        inline bool IsFlagRaised(Flag flag) const { return (m_Flags & static_cast<char>(flag)); }
        inline void SetFlags(char flags) { m_Flags = flags; }

        inline size_t GetCurrentInstruction() const { return m_CurrentInstruction; }
        inline void JumpToInstruction(size_t nextInstruction) { m_CurrentInstruction = nextInstruction; }

    private:
        using InstructionCallback = void(*)(VMContext&);

        InstructionCallback GetInstruction(Instruction instruction) const;
        void* GetMemoryLocationPointerAtCurrentInstructionInternal(size_t elementSize);

        static constexpr size_t K_GENERAL_PURPOSE_REGISTER_COUNT{ static_cast<size_t>(MemoryLocation::RegisterLast) - static_cast<size_t>(MemoryLocation::RegisterFirst) + 1 };
        static constexpr size_t K_FLOATING_POINT_REGISTER_COUNT{ static_cast<size_t>(MemoryLocation::RegisterLast) - static_cast<size_t>(MemoryLocation::RegisterFirst) + 1 };

        long long m_GeneralPurposeRegisters[K_GENERAL_PURPOSE_REGISTER_COUNT];
        InstructionCallback m_Instructions[static_cast<size_t>(Instruction::Count)];
        char* m_RawMemory;
        size_t m_RawMemorySize;
        size_t m_CurrentInstruction;
        char m_Flags;
    };
}