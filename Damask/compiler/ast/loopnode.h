#pragma once

#include <compiler/ast/node.h>

namespace AST
{
    class LoopNode : public Node
    {
    public:
        LoopNode();

        inline Node* GetConditionExpression() { return m_ConditionExpression; }
        inline Node* GetLoopBody() { return m_LoopBody; }

        void SetConditionExpression(Node* conditionExpression);
        void SetLoopBody(Node* loopBody);

        void Accept(ASTVisitor& visitor) override;

    private:
        Node* m_ConditionExpression;
        Node* m_LoopBody;
    };
}