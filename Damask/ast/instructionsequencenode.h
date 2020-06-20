#pragma once

#include <ast/node.h>

namespace AST
{
    class InstructionSequenceNode : public Node
    {
    public:
        void AddInstruction(Node* instruction);

        void Accept(ASTVisitor& visitor) override;
    };
}