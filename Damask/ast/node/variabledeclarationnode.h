#pragma once

#include <ast/node/node.h>

#include <string_view>

enum class EToken;

namespace AST
{
    class VariableDeclarationNode : public Node
    {
    public:
        VariableDeclarationNode();

        inline std::string_view GetVariableName() const { return m_VariableName; }
        inline void SetVariableName(std::string_view variableName) { m_VariableName = variableName; }

        inline EToken GetVariableType() const { return m_VariableType; }
        inline void SetVariableType(EToken variableType) { m_VariableType = variableType; }

        inline Node* GetInitExpression() { return m_InitExpression; }
        void SetInitExpression(Node* initExpression);

        void Accept(ASTVisitor& visitor) override;

    private:
        std::string_view m_VariableName;
        EToken m_VariableType;
        Node* m_InitExpression;
    };
}