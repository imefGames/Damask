#pragma once

namespace AST
{
    class BranchNode;
    class FunctionCallNode;
    class InstructionSequenceNode;
    class LoopNode;
    class OperatorNode;
    class RawValueNode;
    class VariableDeclarationNode;

    class ASTVisitor
    {
    public:
        virtual void VisitNode(BranchNode& node) {}
        virtual void VisitNode(FunctionCallNode& node) {}
        virtual void VisitNode(InstructionSequenceNode& node) {}
        virtual void VisitNode(LoopNode& node) {}
        virtual void VisitNode(OperatorNode& node) {}
        virtual void VisitNode(RawValueNode& node) {}
        virtual void VisitNode(VariableDeclarationNode& node) {}
    };
}