#include <compiler/ast/branchnode.h>

#include <compiler/ast/visitor/astvisitor.h>

namespace AST
{
    BranchNode::BranchNode()
        : m_ConditionExpression{ nullptr }
        , m_IfBody{ nullptr }
        , m_ElseBody{ nullptr }
    {
    }

    void BranchNode::SetConditionExpression(Node* conditionExpression)
    {
        m_ConditionExpression = conditionExpression;
        m_Children.push_back(conditionExpression);
    }

    void BranchNode::SetIfBody(Node* ifBody)
    {
        m_IfBody = ifBody;
        m_Children.push_back(ifBody);
    }

    void BranchNode::SetElseBody(Node* elseBody)
    {
        m_ElseBody= elseBody;
        m_Children.push_back(elseBody);
    }

    void BranchNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}