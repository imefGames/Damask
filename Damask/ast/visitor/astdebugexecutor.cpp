#include <ast/visitor/astdebugexecutor.h>

#include <ast/branchnode.h>
#include <ast/functioncallnode.h>
#include <ast/instructionsequencenode.h>
#include <ast/operatornode.h>
#include <ast/rawvaluenode.h>
#include <ast/variabledeclarationnode.h>

#include <lexer.h>

#include <iostream>

namespace AST
{
    ASTDebugExecutor::ASTDebugExecutor()
        : m_ReturnValue{ 0 }
    {
    }

    void ASTDebugExecutor::VisitNode(BranchNode& node)
    {
    }

    void ASTDebugExecutor::VisitNode(FunctionCallNode& node)
    {
        if (node.GetFunctionName() == "ReadS32")
        {
            std::cout << '>';
            std::cin >> m_ReturnValue;
        }
        else if (node.GetFunctionName() == "Print")
        {
            for (Node* child : node.GetChildrenNodes())
            {
                child->Accept(*this);
            }
            std::cout << m_ReturnValue << '\n';
        }
    }

    void ASTDebugExecutor::VisitNode(InstructionSequenceNode& node)
    {
        for (Node* child : node.GetChildrenNodes())
        {
            child->Accept(*this);
        }
    }

    void ASTDebugExecutor::VisitNode(OperatorNode& node)
    {
        int rhsValue{ 0 };
        int lhsValue{ 0 };

        if (Node* lhs = node.GetLHSNode())
        {
            lhs->Accept(*this);
            lhsValue = m_ReturnValue;
        }

        if (Node* rhs = node.GetRHSNode())
        {
            rhs->Accept(*this);
            rhsValue = m_ReturnValue;
        }

        switch (node.GetOperatorType())
        {
            case EToken::OperatorAdd:
            {
                m_ReturnValue = lhsValue + rhsValue;
                break;
            }

            case EToken::OperatorSub:
            {
                m_ReturnValue = lhsValue - rhsValue;
                break;
            }

            case EToken::OperatorMul:
            {
                m_ReturnValue = lhsValue * rhsValue;
                break;
            }

            case EToken::OperatorDiv:
            {
                m_ReturnValue = lhsValue / rhsValue;
                break;
            }

            case EToken::OperatorMod:
            {
                m_ReturnValue = lhsValue % rhsValue;
                break;
            }
        }
    }

    void ASTDebugExecutor::VisitNode(RawValueNode& node)
    {
        std::string value{ node.GetValue() };
        auto foundIt{ m_Variables.find(value) };
        if (foundIt != m_Variables.end())
        {
            m_ReturnValue = foundIt->second;
        }
        else
        {
            m_ReturnValue = atoi(value.c_str());
        }
    }

    void ASTDebugExecutor::VisitNode(VariableDeclarationNode& node)
    {
        m_ReturnValue = 0;
        if (Node* init = node.GetInitExpression())
        {
            init->Accept(*this);
        }
        std::string variableName{ node.GetVariableName() };
        m_Variables[variableName] = m_ReturnValue;
    }
}