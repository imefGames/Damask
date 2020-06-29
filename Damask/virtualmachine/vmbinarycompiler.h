#pragma once

#include <compiler/ast/visitor/astvisitor.h>
#include <vector>

namespace VirtualMachine
{
    class VMBinaryCompiler : public AST::ASTVisitor
    {
    public:
        void VisitNode(AST::BranchNode& node) override;
        void VisitNode(AST::FunctionCallNode& node) override;
        void VisitNode(AST::FunctionDeclarationNode& node) override;
        void VisitNode(AST::InstructionSequenceNode& node) override;
        void VisitNode(AST::LoopNode& node) override;
        void VisitNode(AST::OperatorNode& node) override;
        void VisitNode(AST::RawValueNode& node) override;
        void VisitNode(AST::VariableDeclarationNode& node) override;

        std::vector<char> BuildBinaryCode() const;

    private:
        std::vector<char> m_TextSegment;
        std::vector<char> m_DataSegment;
        std::vector<char> m_BSSSegment;
    };
}