#include <ast/functioncallnode.h>

#include <lexer.h>
#include <ast/visitor/astvisitor.h>

namespace AST
{
    FunctionCallNode::FunctionCallNode()
        : m_ReturnType{ EToken::None }
    {
    }

    void FunctionCallNode::AddArgument(Node* argumentNode)
    {
        m_Children.push_back(argumentNode);
    }

    void FunctionCallNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}