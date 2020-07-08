#include <ast/node/rawvaluenode.h>

#include <ast/node/visitor/astvisitor.h>

namespace AST
{
    RawValueNode::RawValueNode(std::string_view value)
        : m_Value{ value }
    {
    }

    void RawValueNode::Accept(ASTVisitor& visitor)
    {
        visitor.VisitNode(*this);
    }
}