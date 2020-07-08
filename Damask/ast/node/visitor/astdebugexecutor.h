#pragma once

#include <ast/node/visitor/astvisitor.h>
#include <map>
#include <string>

class CompilationContext;

namespace AST
{
    class Node;

    class ASTDebugExecutor : public ASTVisitor
    {
    public:
        ASTDebugExecutor(CompilationContext& compilationContext);

        void VisitNode(BranchNode& node) override;
        void VisitNode(FunctionCallNode& node) override;
        void VisitNode(FunctionDeclarationNode& node) override;
        void VisitNode(InstructionSequenceNode& node) override;
        void VisitNode(LoopNode& node) override;
        void VisitNode(OperatorNode& node) override;
        void VisitNode(RawValueNode& node) override;
        void VisitNode(VariableDeclarationNode& node) override;

    private:
        std::map<std::string, int> m_Variables;
        CompilationContext& m_CompilationContext;
        int m_ReturnValue;
    };
}