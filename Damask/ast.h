#pragma once


class ExecutionContext;
class Lexer;

namespace AST
{
	class Node;

	Node* BuildAST(Lexer& lexern, ExecutionContext& executionContext);
}