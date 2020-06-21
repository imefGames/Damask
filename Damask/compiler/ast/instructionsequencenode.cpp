#include <compiler/ast/instructionsequencenode.h>

#include <compiler/ast/visitor/astvisitor.h>

namespace AST
{
    void InstructionSequenceNode::AddInstruction(Node* instruction)
    {
        m_Children.push_back(instruction);
    }

    void InstructionSequenceNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}