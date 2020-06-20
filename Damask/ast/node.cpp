#include <ast/node.h>

namespace AST
{
	Node::~Node()
	{
		for (Node* node : m_Children)
		{
			delete node;
		}
		m_Children.clear();
	}
}