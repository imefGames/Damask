#include <virtualmachine/vmbinarycompiler.h>

#include <virtualmachine/instructions.h>

namespace VirtualMachine
{
    namespace Internal
    {
        template <class T>
        void PushDataToSegment(std::vector<char>& segment, const T& data)
        {
            //TODO
        }
    }

    void VMBinaryCompiler::VisitNode(AST::BranchNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::FunctionCallNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::FunctionDeclarationNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::InstructionSequenceNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::LoopNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::OperatorNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::RawValueNode& node)
    {
    }

    void VMBinaryCompiler::VisitNode(AST::VariableDeclarationNode& node)
    {
    }

    std::vector<char> VMBinaryCompiler::BuildBinaryCode() const
    {
        std::vector<char> assembledBinary{};
        assembledBinary.reserve(m_TextSegment.size() + m_DataSegment.size() + m_BSSSegment.size());

        assembledBinary.insert(assembledBinary.end(), m_TextSegment.begin(), m_TextSegment.end());
        assembledBinary.insert(assembledBinary.end(), m_DataSegment.begin(), m_DataSegment.end());
        assembledBinary.insert(assembledBinary.end(), m_BSSSegment.begin(), m_BSSSegment.end());
        //TODO: separate segments with some NOP instructions.

        return assembledBinary;
    }
}