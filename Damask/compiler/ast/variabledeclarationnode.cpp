#include <compiler/ast/variabledeclarationnode.h>

#include <compiler/lexer.h>
#include <compiler/ast/visitor/astvisitor.h>

namespace AST
{
    VariableDeclarationNode::VariableDeclarationNode()
        : m_VariableType{ EToken::None }
        , m_InitExpression{ nullptr }
    {
    }

    void VariableDeclarationNode::SetInitExpression(Node* initExpression)
    {
        m_InitExpression = initExpression;
        m_Children.push_back(initExpression);
    }

    void VariableDeclarationNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}