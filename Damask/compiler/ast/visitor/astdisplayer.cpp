#include <compiler/ast/visitor/astdisplayer.h>

#include <compiler/ast/branchnode.h>
#include <compiler/ast/functioncallnode.h>
#include <compiler/ast/functiondeclarationnode.h>
#include <compiler/ast/instructionsequencenode.h>
#include <compiler/ast/loopnode.h>
#include <compiler/ast/operatornode.h>
#include <compiler/ast/rawvaluenode.h>
#include <compiler/ast/variabledeclarationnode.h>

#include <iostream>

namespace AST
{
    ASTDisplayer::ASTDisplayer()
        :m_Indentation{ 0 }
    {
    }

    void ASTDisplayer::VisitNode(BranchNode& node)
    {
        DisplayText("Branch:");
        PushIndentation();

        DisplayText("Condition:");
        DisplayNode(node.GetConditionExpression());
        
        DisplayText("If Body:");
        DisplayNode(node.GetIfBody());

        DisplayText("Else Body:");
        DisplayNode(node.GetElseBody());

        PopIndentation();
    }

    void ASTDisplayer::VisitNode(FunctionCallNode& node)
    {
        DisplayText("Function Call:");
        PushIndentation();

        DisplayText(node.GetFunctionName());

        for (Node* child : node.GetChildrenNodes())
        {
            DisplayNode(child);
        }

        PopIndentation();
    }
    
    void ASTDisplayer::VisitNode(FunctionDeclarationNode& node)
    {
        DisplayText("Function Declaration:");
        PushIndentation();

        DisplayText(node.GetFunctionName());
        //TODO: display return type

        DisplayText("Arguments:");
        for (Node* child : node.GetArguments())
        {
            DisplayNode(child);
        }

        DisplayText("Function Body:");
        DisplayNode(node.GetFunctionBody());

        PopIndentation();
    }

    void ASTDisplayer::VisitNode(InstructionSequenceNode& node)
    {
        DisplayText("Sequence:");
        PushIndentation();

        for (Node* child : node.GetChildrenNodes())
        {
            DisplayNode(child);
        }

        PopIndentation();
    }

    void ASTDisplayer::VisitNode(LoopNode& node)
    {
        DisplayText("Loop:");
        PushIndentation();

        DisplayText("Condition:");
        DisplayNode(node.GetConditionExpression());

        DisplayText("Loop Body:");
        DisplayNode(node.GetLoopBody());

        PopIndentation();
    }

    void ASTDisplayer::VisitNode(OperatorNode& node)
    {
        DisplayText("Operator:");
        PushIndentation();
        //TODO: display operator as string
        DisplayText("LHS");
        DisplayNode(node.GetLHSNode());
        DisplayText("RHS");
        DisplayNode(node.GetRHSNode());
        PopIndentation();
    }

    void ASTDisplayer::VisitNode(RawValueNode& node)
    {
        DisplayText("Raw Value:");
        PushIndentation();
        DisplayText(node.GetValue());
        PopIndentation();
    }

    void ASTDisplayer::VisitNode(VariableDeclarationNode& node)
    {
        DisplayText("Variable Declaration:");
        PushIndentation();
        DisplayText(node.GetVariableName());
        //TODO: display type
        DisplayNode(node.GetInitExpression());
        PopIndentation();
    }

    void ASTDisplayer::DisplayText(std::string_view text)
    {
        std::cout << m_Indentation << text << '\n';
    }

    void ASTDisplayer::DisplayNode(Node* node)
    {
        if (node != nullptr)
        {
            node->Accept(*this);
        }
        else
        {
            std::cout << m_Indentation << "null\n";
        }
    }

    void ASTDisplayer::PushIndentation()
    {
        int size{ (int)m_Indentation.length() + 4 };
        if (size < 0) { size = 0; }
        m_Indentation = std::string(size, ' ');
    }

    void ASTDisplayer::PopIndentation()
    {
        int size{ (int)m_Indentation.length() - 4 };
        if (size < 0) { size = 0; }
        m_Indentation = std::string(size, ' ');
    }
}