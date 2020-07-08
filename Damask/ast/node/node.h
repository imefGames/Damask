#pragma once

#include <vector>

namespace AST
{
	class ASTVisitor;

	class Node
	{
	public:
		virtual ~Node();

		virtual void Accept(ASTVisitor& visitor) = 0;

		inline const std::vector<Node*>& GetChildrenNodes() const { return m_Children; }

	protected:
		std::vector<Node*> m_Children;
	};
}