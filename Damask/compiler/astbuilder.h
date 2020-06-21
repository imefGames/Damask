#pragma once

class CompilationContext;
struct Lexer;

namespace AST
{
	class Node;

	Node* BuildAST(Lexer& lexern, CompilationContext& compilationContext);
}