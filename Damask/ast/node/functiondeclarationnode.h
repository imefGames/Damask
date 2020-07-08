#pragma once

#include <ast/node/node.h>
#include <string_view>

enum class EToken;

namespace AST
{
    class VariableDeclarationNode;

    class FunctionDeclarationNode : public Node
    {
    public:
        FunctionDeclarationNode();

        inline const std::vector<VariableDeclarationNode*>& GetArguments() const { return m_Arguments; }
        inline std::string_view GetFunctionName() const { return m_FunctionName; }
        inline EToken GetReturnType() const { return m_ReturnType; }
        inline Node* GetFunctionBody() const { return m_FunctionBody; }

        inline void SetFunctionName(std::string_view functionName) { m_FunctionName = functionName; }
        inline void SetReturnType(EToken returnType) { m_ReturnType = returnType; }
        void SetFunctionBody(Node* functionBody);

        void AddArgument(VariableDeclarationNode* argumentNode);

        void Accept(ASTVisitor& visitor) override;

    private:
        std::vector<VariableDeclarationNode*> m_Arguments;
        std::string_view m_FunctionName;
        EToken m_ReturnType;
        Node* m_FunctionBody;
    };
}