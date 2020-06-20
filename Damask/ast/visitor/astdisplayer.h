#pragma once

#include <ast/visitor/astvisitor.h>
#include <string>
#include <string_view>

namespace AST
{
    class Node;

    class ASTDisplayer : public ASTVisitor
    {
    public:
        ASTDisplayer();

        void VisitNode(BranchNode& node) override;
        void VisitNode(FunctionCallNode& node) override;
        void VisitNode(InstructionSequenceNode& node) override;
        void VisitNode(OperatorNode& node) override;
        void VisitNode(RawValueNode& node) override;
        void VisitNode(VariableDeclarationNode& node) override;

    private:
        void DisplayText(std::string_view text);
        void DisplayNode(Node* node);

        void PushIndentation();
        void PopIndentation();

        std::string m_Indentation;
    };
}