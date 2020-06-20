#pragma once

#include <ast/visitor/astvisitor.h>
#include <map>
#include <string>

namespace AST
{
    class Node;

    class ASTDebugExecutor : public ASTVisitor
    {
    public:
        ASTDebugExecutor();

        void VisitNode(BranchNode& node) override;
        void VisitNode(FunctionCallNode& node) override;
        void VisitNode(InstructionSequenceNode& node) override;
        void VisitNode(OperatorNode& node) override;
        void VisitNode(RawValueNode& node) override;
        void VisitNode(VariableDeclarationNode& node) override;

    private:
        std::map<std::string, int> m_Variables;
        int m_ReturnValue;
    };
}