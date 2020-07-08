#include <ast/astbuilder.h>

#include <ast/compilationcontext.h>
#include <ast/lexer.h>
#include <ast/node/branchnode.h>
#include <ast/node/functioncallnode.h>
#include <ast/node/functiondeclarationnode.h>
#include <ast/node/instructionsequencenode.h>
#include <ast/node/loopnode.h>
#include <ast/node/operatornode.h>
#include <ast/node/rawvaluenode.h>
#include <ast/node/variabledeclarationnode.h>

#include <stack>

namespace AST
{
	namespace Internal
	{
		VariableDeclarationNode* BuildArgumentDeclaration(Lexer& lexer, CompilationContext& compilationContext, const Token& typeToken);
		Node* BuildAssignation(Lexer& lexer, CompilationContext& compilationContext, const Token& assignee);
		Node* BuildBranch(Lexer& lexer, CompilationContext& compilationContext);
		Node* BuildDeclaration(Lexer& lexer, CompilationContext& compilationContext, const Token& typeToken);
		Node* BuildExpression(Lexer& lexer, CompilationContext& compilationContext);
		Node* BuildFunctionCall(Lexer& lexer, CompilationContext& compilationContext, const Token& calledFunction);
		Node* BuildLoop(Lexer& lexer, CompilationContext& compilationContext);
		Node* BuildScopedInstructionSequence(Lexer& lexer, CompilationContext& compilationContext);

		void Expect(Lexer& lexer, CompilationContext& compilationContext, EToken expectedToken)
		{
			Token nextToken{ lexer.ReadNextToken() };
			if (nextToken.TokenType != expectedToken)
			{
				// TODO: push error
			}
		}

		static Node* BuildInstruction(Lexer& lexer, CompilationContext& compilationContext)
		{
			Node* node{ nullptr };
			if (lexer.HasUnreadText())
			{
				Token nextToken{ lexer.ReadNextToken() };
				switch (nextToken.TokenType)
				{
					case EToken::SeparatorLBracketCurly:
					{
						node = BuildScopedInstructionSequence(lexer, compilationContext);
						break;
					}

					case EToken::TypeNameVoid:
					case EToken::TypeNameS32:
					case EToken::TypeNameU32:
					{
						node = BuildDeclaration(lexer, compilationContext, nextToken);
						break;
					}

					case EToken::KeywordIf:
					{
						node = BuildBranch(lexer, compilationContext);
						break;
					}

					case EToken::KeywordDo:
					case EToken::KeywordFor:
					{
						// TODO: error
						break;
					}

					case EToken::KeywordWhile:
					{
						node = BuildLoop(lexer, compilationContext);
						break;
					}

					case EToken::Identifier:
					{
						Token identifierToken{ nextToken };
						nextToken = lexer.ReadNextToken();

						switch (nextToken.TokenType)
						{
							case EToken::SeparatorLBracketRound:
							{
								node = BuildFunctionCall(lexer, compilationContext, identifierToken);
								Expect(lexer, compilationContext, EToken::SeparatorInstructionEnd);
								break;
							}

							case EToken::OperatorAssign:
							{
								node = BuildAssignation(lexer, compilationContext, identifierToken);
								Expect(lexer, compilationContext, EToken::SeparatorInstructionEnd);
								break;
							}

							default:
							{
								// TODO: error
								break;
							}
						}
						break;
					}
				}
			}
			return node;
		}

		VariableDeclarationNode* BuildArgumentDeclaration(Lexer& lexer, CompilationContext& compilationContext, const Token& typeToken)
		{
			Token indentifierToken = lexer.ReadNextToken();
			VariableDeclarationNode* variableDeclaration{ new VariableDeclarationNode{} };
			variableDeclaration->SetVariableName(indentifierToken.TokenText);
			variableDeclaration->SetVariableType(typeToken.TokenType);
			return variableDeclaration;
		}

		Node* BuildAssignation(Lexer& lexer, CompilationContext& compilationContext, const Token& assignee)
		{
			OperatorNode* operatorNode{ new OperatorNode{} };
			operatorNode->SetOperatorType(EToken::OperatorAssign);
			operatorNode->SetLHSNode(new RawValueNode{ assignee.TokenText });
			operatorNode->SetRHSNode(BuildExpression(lexer, compilationContext));
			return operatorNode;
		}

		Node* BuildBranch(Lexer& lexer, CompilationContext& compilationContext)
		{
			// TODO: Handle errors

			BranchNode* branchNode{ new BranchNode{} };
			Expect(lexer, compilationContext, EToken::SeparatorLBracketRound);
			branchNode->SetConditionExpression(BuildExpression(lexer, compilationContext));
			Expect(lexer, compilationContext, EToken::SeparatorRBracketRound);
			branchNode->SetIfBody(BuildInstruction(lexer, compilationContext));
			if (lexer.PeekNextToken().TokenType == EToken::KeywordElse)
			{
				lexer.ReadNextToken();
				branchNode->SetElseBody(BuildInstruction(lexer, compilationContext));
			}
			return branchNode;
		}

		Node* BuildDeclaration(Lexer& lexer, CompilationContext& compilationContext, const Token& typeToken)
		{
			Node* declarationToken{ nullptr };
			Token indentifierToken = lexer.ReadNextToken();
			if (indentifierToken.TokenType == EToken::Identifier)
			{
				Token nextToken = lexer.PeekNextToken();
				switch (nextToken.TokenType)
				{
					case EToken::SeparatorLBracketRound:
					{
						lexer.ReadNextToken();

						FunctionDeclarationNode* functionDeclarationNode{ new FunctionDeclarationNode{} };
						functionDeclarationNode->SetFunctionName(indentifierToken.TokenText);
						functionDeclarationNode->SetReturnType(typeToken.TokenType);

						bool lookingForArguments{ true };
						while (lookingForArguments)
						{
							Token currentToken{ lexer.ReadNextToken() };
							if (currentToken.TokenType == EToken::SeparatorRBracketRound)
							{
								lookingForArguments = false;
							}
							else
							{
								functionDeclarationNode->AddArgument(BuildArgumentDeclaration(lexer, compilationContext, currentToken));
								Token followingToken{ lexer.ReadNextToken() };
								
								if (followingToken.TokenType == EToken::SeparatorRBracketRound)
								{
									lookingForArguments = false;
								}
								else if (followingToken.TokenType != EToken::SeparatorComma)
								{
									//TODO: error
								}
							}
						}

						Expect(lexer, compilationContext, EToken::SeparatorLBracketCurly);
						functionDeclarationNode->SetFunctionBody(BuildScopedInstructionSequence(lexer, compilationContext));
						compilationContext.RegisterFunction(std::string{ indentifierToken.TokenText }, functionDeclarationNode);
						break;
					}

					case EToken::OperatorAssign:
					{
						lexer.ReadNextToken();

						VariableDeclarationNode* variableDeclaration{ new VariableDeclarationNode{} };
						variableDeclaration->SetVariableName(indentifierToken.TokenText);
						variableDeclaration->SetVariableType(indentifierToken.TokenType);
						variableDeclaration->SetInitExpression(BuildExpression(lexer, compilationContext));
						Expect(lexer, compilationContext, EToken::SeparatorInstructionEnd);
						declarationToken = variableDeclaration;
						break;
					}

					case EToken::SeparatorInstructionEnd:
					{
						lexer.ReadNextToken();

						VariableDeclarationNode* variableDeclaration{ new VariableDeclarationNode{} };
						variableDeclaration->SetVariableName(indentifierToken.TokenText);
						variableDeclaration->SetVariableType(indentifierToken.TokenType);
						declarationToken = variableDeclaration;
						break;
					}

					default:
					{
						// TODO: error
						break;
					}
				}
			}
			else
			{
				//TODO: error
			}
			return declarationToken;
		}

		Node* BuildExpressionTreeRecursive(std::stack<Node*>& tokenStack)
		{
			Node* topNode{ nullptr };
			if (!tokenStack.empty())
			{
				topNode = tokenStack.top();
				tokenStack.pop();

				if (OperatorNode* topOperator = dynamic_cast<OperatorNode*>(topNode))
				{
					topOperator->SetRHSNode(BuildExpressionTreeRecursive(tokenStack));
					topOperator->SetLHSNode(BuildExpressionTreeRecursive(tokenStack));
				}
			}
			return topNode;
		}

		Node* BuildExpression(Lexer& lexer, CompilationContext& compilationContext)
		{
			std::stack<EToken> operatorstack;
			std::stack<Node*> tokenStack;
		
			bool keepParsingExpression{ true };
			while (keepParsingExpression && lexer.HasUnreadText())
			{
				Token currentToken{ lexer.PeekNextToken() };
				switch (currentToken.TokenType)
				{
					case EToken::SeparatorLBracketRound:
					{
						operatorstack.push(currentToken.TokenType);
						lexer.ReadNextToken();
						break;
					}

					case EToken::SeparatorRBracketRound:
					{
						bool foundStartBracket{ false };
						while (!operatorstack.empty() && !foundStartBracket)
						{
							if (operatorstack.top() == EToken::SeparatorLBracketRound)
							{
								foundStartBracket = true;
							}
							else
							{
								OperatorNode* operatorNode{ new OperatorNode{} };
								operatorNode->SetOperatorType(operatorstack.top());
								tokenStack.push(operatorNode);
							}
							operatorstack.pop();
						}
						
						if (!foundStartBracket)
						{
							keepParsingExpression = false;
						}
						else
						{
							lexer.ReadNextToken();
						}

						break;
					}

					case EToken::Identifier:
					{
						lexer.ReadNextToken();

						Token followingToken{ lexer.PeekNextToken() };
						if (followingToken.TokenType == EToken::SeparatorLBracketRound)
						{
							lexer.ReadNextToken();
							tokenStack.push(BuildFunctionCall(lexer, compilationContext, currentToken));
						}
						else
						{
							tokenStack.push(new RawValueNode{ currentToken.TokenText });
						}

						break;
					}

					case EToken::LiteralBool:
					case EToken::LiteralChar:
					case EToken::LiteralNumber:
					case EToken::LiteralString:
					{
						tokenStack.push(new RawValueNode{ currentToken.TokenText });
						lexer.ReadNextToken();
						break;
					}

					case EToken::OperatorAdd:
					case EToken::OperatorSub:
					case EToken::OperatorMul:
					case EToken::OperatorDiv:
					case EToken::OperatorMod:
					case EToken::OperatorAssign:
					{
						//TODO: handle precedence
						operatorstack.push(currentToken.TokenType);
						lexer.ReadNextToken();
						break;
					}

					default:
					{
						keepParsingExpression = false;
						break;
					}
				}
			}

			while (!operatorstack.empty())
			{
				OperatorNode* operatorNode{ new OperatorNode{} };
				operatorNode->SetOperatorType(operatorstack.top());
				tokenStack.push(operatorNode);
				operatorstack.pop();
			}

			return BuildExpressionTreeRecursive(tokenStack);
		}

		Node* BuildFunctionCall(Lexer& lexer, CompilationContext& compilationContext, const Token& calledFunction)
		{
			FunctionCallNode* functionCallNode{ new FunctionCallNode{} };
			functionCallNode->SetFunctionName(calledFunction.TokenText);

			bool lookingForArguments{ true };
			while (lookingForArguments)
			{
				Token currentToken{ lexer.PeekNextToken() };
				if (currentToken.TokenType == EToken::SeparatorRBracketRound)
				{
					lexer.ReadNextToken();
					lookingForArguments = false;
				}
				else
				{
					functionCallNode->AddArgument(BuildExpression(lexer, compilationContext));
					Token followingToken{ lexer.PeekNextToken() };
					if (followingToken.TokenType != EToken::SeparatorRBracketRound && followingToken.TokenType != EToken::SeparatorComma)
					{
						//TODO: error
					}
				}
			}
			return functionCallNode;
		}

		Node* BuildLoop(Lexer& lexer, CompilationContext& compilationContext)
		{
			// TODO: Handle errors

			LoopNode* loopNode{ new LoopNode{} };
			Expect(lexer, compilationContext, EToken::SeparatorLBracketRound);
			loopNode->SetConditionExpression(BuildExpression(lexer, compilationContext));
			Expect(lexer, compilationContext, EToken::SeparatorRBracketRound);
			loopNode->SetLoopBody(BuildInstruction(lexer, compilationContext));
			return loopNode;
		}

		Node* BuildScopedInstructionSequence(Lexer& lexer, CompilationContext& compilationContext)
		{
			InstructionSequenceNode* instructionSequence{ new InstructionSequenceNode{} };
			while (lexer.PeekNextToken().TokenType != EToken::SeparatorRBracketCurly)
			{
				if (Node* builtNode = Internal::BuildInstruction(lexer, compilationContext))
				{
					instructionSequence->AddInstruction(builtNode);
				}
				else
				{
					// TODO: error
					break;
				}
			}
			lexer.ReadNextToken();
			return instructionSequence;
		}
	}

	Node* BuildAST(Lexer& lexer, CompilationContext& compilationContext)
	{
		InstructionSequenceNode* instructionSequence{ new InstructionSequenceNode{} };

		while (lexer.HasUnreadText()) // TODO: break if an error occurs
		{
			if (Node* builtNode = Internal::BuildInstruction(lexer, compilationContext))
			{
				instructionSequence->AddInstruction(builtNode);
			}
		}

		return instructionSequence;
	}
}