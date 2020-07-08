#include <ast/node/functiondeclarationnode.h>

#include <ast/lexer.h>
#include <ast/node/variabledeclarationnode.h>
#include <ast/node/visitor/astvisitor.h>

namespace AST
{
    FunctionDeclarationNode::FunctionDeclarationNode()
        : m_ReturnType{ EToken::None }
        , m_FunctionBody{ nullptr }
    {
    }

    void FunctionDeclarationNode::AddArgument(VariableDeclarationNode* argumentNode)
    {
        m_Arguments.push_back(argumentNode);
        m_Children.push_back(argumentNode);
    }

    void FunctionDeclarationNode::SetFunctionBody(Node* functionBody)
    {
        m_FunctionBody = functionBody;
        m_Children.push_back(functionBody);
    }

    void FunctionDeclarationNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}