#pragma once


class ExecutionContext;
struct Lexer;

namespace AST
{
	class Node;

	Node* BuildAST(Lexer& lexern, ExecutionContext& executionContext);
}