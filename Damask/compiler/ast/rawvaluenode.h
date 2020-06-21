#pragma once

#include <compiler/ast/node.h>

#include <string_view>

enum class EToken;

namespace AST
{
    class RawValueNode : public Node
    {
    public:
        RawValueNode(std::string_view value);

        inline std::string_view GetValue() const { return m_Value; }
        inline void SetValue(std::string_view value) { m_Value = value; }

        void Accept(ASTVisitor& visitor) override;

    private:
        std::string_view m_Value;
    };
}