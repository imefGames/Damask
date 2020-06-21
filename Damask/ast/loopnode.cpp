#include <ast/loopnode.h>

#include <ast/visitor/astvisitor.h>

namespace AST
{
    LoopNode::LoopNode()
        : m_ConditionExpression{ nullptr }
        , m_LoopBody{ nullptr }
    {
    }

    void LoopNode::SetConditionExpression(Node* conditionExpression)
    {
        m_ConditionExpression = conditionExpression;
        m_Children.push_back(conditionExpression);
    }

    void LoopNode::SetLoopBody(Node* loopBody)
    {
        m_LoopBody = loopBody;
        m_Children.push_back(loopBody);
    }

    void LoopNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}