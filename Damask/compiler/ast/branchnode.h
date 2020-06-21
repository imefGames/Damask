#pragma once

#include <compiler/ast/node.h>

namespace AST
{
    class BranchNode : public Node
    {
    public:
        BranchNode();

        inline Node* GetConditionExpression() { return m_ConditionExpression; }
        inline Node* GetIfBody() { return m_IfBody; }
        inline Node* GetElseBody() { return m_ElseBody; }

        void SetConditionExpression(Node* conditionExpression);
        void SetIfBody(Node* ifBody);
        void SetElseBody(Node* elseBody);

        void Accept(ASTVisitor& visitor) override;

    private:
        Node* m_ConditionExpression;
        Node* m_IfBody;
        Node* m_ElseBody;
    };
}