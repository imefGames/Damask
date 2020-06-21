#pragma once

#include <compiler/ast/node.h>
#include <string_view>

enum class EToken;

namespace AST
{
    class FunctionCallNode : public Node
    {
    public:
        FunctionCallNode();

        inline std::string_view GetFunctionName() const { return m_FunctionName; }
        inline EToken GetReturnType() const { return m_ReturnType; }

        inline void SetFunctionName(std::string_view functionName) { m_FunctionName = functionName; }
        inline void SetReturnType(EToken returnType) { m_ReturnType = returnType; }

        void AddArgument(Node* argumentNode);

        void Accept(ASTVisitor& visitor) override;

    private:
        std::string_view m_FunctionName;
        EToken m_ReturnType;
    };
}