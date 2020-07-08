#include <ast/node/functioncallnode.h>

#include <ast/lexer.h>
#include <ast/node/visitor/astvisitor.h>

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