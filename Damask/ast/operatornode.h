#pragma once

#include <ast/node.h>

enum class EToken;

namespace AST
{
    class OperatorNode : public Node
    {
    public:
        OperatorNode();

        inline EToken GetOperatorType() const { return m_OperatorType; }
        inline Node* GetLHSNode() { return m_LHSNode; }
        inline Node* GetRHSNode() { return m_RHSNode; }

        void SetOperatorType(EToken operatorType);
        void SetLHSNode(Node* lhsNode);
        void SetRHSNode(Node* rhsNode);

        void Accept(ASTVisitor& visitor) override;

    private:
        EToken m_OperatorType;
        Node* m_LHSNode;
        Node* m_RHSNode;
    };
}