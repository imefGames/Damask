#include <ast/node/operatornode.h>

#include <ast/lexer.h>
#include <ast/node/visitor/astvisitor.h>

namespace AST
{
    OperatorNode::OperatorNode()
        : m_OperatorType{ EToken::None }
        , m_LHSNode{ nullptr }
        , m_RHSNode{ nullptr }
    {
    }

    void OperatorNode::SetOperatorType(EToken operatorType)
    {
        m_OperatorType = operatorType;
    }

    void OperatorNode::SetLHSNode(Node* lhsNode)
    {
        m_LHSNode = lhsNode;
        m_Children.push_back(lhsNode);
    }

    void OperatorNode::SetRHSNode(Node* rhsNode)
    {
        m_RHSNode = rhsNode;
        m_Children.push_back(rhsNode);
    }

    void OperatorNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}