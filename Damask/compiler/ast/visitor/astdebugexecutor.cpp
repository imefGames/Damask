#include <compiler/ast/visitor/astdebugexecutor.h>

#include <compiler/compilationcontext.h>
#include <compiler/ast/branchnode.h>
#include <compiler/ast/functioncallnode.h>
#include <compiler/ast/functiondeclarationnode.h>
#include <compiler/ast/instructionsequencenode.h>
#include <compiler/ast/loopnode.h>
#include <compiler/ast/operatornode.h>
#include <compiler/ast/rawvaluenode.h>
#include <compiler/ast/variabledeclarationnode.h>

#include <compiler/lexer.h>

#include <iostream>

namespace AST
{
    ASTDebugExecutor::ASTDebugExecutor(CompilationContext& compilationContext)
        : m_CompilationContext{ compilationContext }
        , m_ReturnValue{ 0 }
    {
    }

    void ASTDebugExecutor::VisitNode(BranchNode& node)
    {
        m_ReturnValue = 0;
        if (Node* condition = node.GetConditionExpression())
        {
            condition->Accept(*this);
        }

        if (m_ReturnValue)
        {
            if (Node* ifBody = node.GetIfBody())
            {
                ifBody->Accept(*this);
            }
        }
        else
        {
            if (Node* elseBody = node.GetElseBody())
            {
                elseBody->Accept(*this);
            }
        }
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
        else if (FunctionDeclarationNode* functionDeclaration = m_CompilationContext.FindFunction(std::string{ node.GetFunctionName() }))
        {
            ASTDebugExecutor functionExecutor{ m_CompilationContext };
            //TODO: check the arguments match.

            const auto& providedArguments{ node.GetChildrenNodes() };
            const auto& expectedArguments{ functionDeclaration->GetArguments() };
            if (providedArguments.size() == expectedArguments.size())
            {
                for (unsigned int i = 0; i < providedArguments.size(); ++i)
                {
                    providedArguments[i]->Accept(*this);
                    functionExecutor.m_Variables[std::string{ expectedArguments[i]->GetVariableName() }] = m_ReturnValue;
                }

                if (Node* body = functionDeclaration->GetFunctionBody())
                {
                    body->Accept(functionExecutor);

                    m_ReturnValue = functionExecutor.m_ReturnValue;
                }
            }
            else
            {
                //TODO: error
            }
        }
    }

    void ASTDebugExecutor::VisitNode(FunctionDeclarationNode& node)
    {
        //TODO: Error! Should never be called.
    }

    void ASTDebugExecutor::VisitNode(InstructionSequenceNode& node)
    {
        for (Node* child : node.GetChildrenNodes())
        {
            child->Accept(*this);
        }
    }

    void ASTDebugExecutor::VisitNode(LoopNode& node)
    {
        m_ReturnValue = 0;
        if (Node* condition = node.GetConditionExpression())
        {
            condition->Accept(*this);
        }

        while (m_ReturnValue)
        {
            if (Node* loopBody = node.GetLoopBody())
            {
                loopBody->Accept(*this);
            }
            if (Node* condition = node.GetConditionExpression())
            {
                condition->Accept(*this);
            }
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

        //TODO: handle assign operator

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