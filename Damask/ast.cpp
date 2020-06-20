#include <ast.h>

#include <executioncontext.h>
#include <lexer.h>
#include <ast/branchnode.h>
#include <ast/functioncallnode.h>
#include <ast/instructionsequencenode.h>
#include <ast/operatornode.h>
#include <ast/rawvaluenode.h>
#include <ast/variabledeclarationnode.h>

#include <stack>

namespace AST
{
	namespace Internal
	{
		Node* BuildAssignation(Lexer& lexer, ExecutionContext& executionContext, const Token& assignee);
		Node* BuildBranch(Lexer& lexer, ExecutionContext& executionContext);
		Node* BuildDeclaration(Lexer& lexer, ExecutionContext& executionContext, const Token& typeToken);
		Node* BuildExpression(Lexer& lexer, ExecutionContext& executionContext);
		Node* BuildFunctionCall(Lexer& lexer, ExecutionContext& executionContext, const Token& calledFunction);
		Node* BuildLoop(Lexer& lexer, ExecutionContext& executionContext);
		Node* BuildScopedInstructionSequence(Lexer& lexer, ExecutionContext& executionContext);

		void Expect(Lexer& lexer, ExecutionContext& executionContext, EToken expectedToken)
		{
			Token nextToken{ lexer.ReadNextToken() };
			if (nextToken.TokenType != expectedToken)
			{
				// TODO: push error
			}
		}

		static Node* BuildInstruction(Lexer& lexer, ExecutionContext& executionContext)
		{
			Node* node{ nullptr };
			if (lexer.HasUnreadText())
			{
				Token nextToken{ lexer.ReadNextToken() };
				switch (nextToken.TokenType)
				{
					case EToken::SeparatorLBracketCurly:
					{
						node = BuildScopedInstructionSequence(lexer, executionContext);
						break;
					}

					case EToken::TypeNameVoid:
					case EToken::TypeNameS32:
					case EToken::TypeNameU32:
					{
						node = BuildDeclaration(lexer, executionContext, nextToken);
						break;
					}

					case EToken::KeywordIf:
					{
						node = BuildBranch(lexer, executionContext);
						break;
					}

					case EToken::KeywordDo:
					case EToken::KeywordFor:
					case EToken::KeywordWhile:
					{
						node = BuildLoop(lexer, executionContext);
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
								node = BuildFunctionCall(lexer, executionContext, identifierToken);
								Expect(lexer, executionContext, EToken::SeparatorInstructionEnd);
								break;
							}

							case EToken::OperatorAssign:
							{
								node = BuildAssignation(lexer, executionContext, identifierToken);
								Expect(lexer, executionContext, EToken::SeparatorInstructionEnd);
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

		Node* BuildAssignation(Lexer& lexer, ExecutionContext& executionContext, const Token& assignee)
		{
			OperatorNode* operatorNode{ new OperatorNode{} };
			operatorNode->SetOperatorType(EToken::OperatorAssign);
			operatorNode->SetLHSNode(new RawValueNode{ assignee.TokenText });
			operatorNode->SetRHSNode(BuildExpression(lexer, executionContext));
			return operatorNode;
		}

		Node* BuildBranch(Lexer& lexer, ExecutionContext& executionContext)
		{
			// TODO: Handle errors

			BranchNode* branchNode{ new BranchNode{} };
			Expect(lexer, executionContext, EToken::SeparatorLBracketRound);
			branchNode->SetConditionExpression(BuildExpression(lexer, executionContext));
			Expect(lexer, executionContext, EToken::SeparatorRBracketRound);
			branchNode->SetIfBody(BuildInstruction(lexer, executionContext));
			if (lexer.PeekNextToken().TokenType == EToken::KeywordElse)
			{
				lexer.ReadNextToken();
				branchNode->SetElseBody(BuildInstruction(lexer, executionContext));
			}
			return branchNode;
		}

		Node* BuildDeclaration(Lexer& lexer, ExecutionContext& executionContext, const Token& typeToken)
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
						// TODO: Function Decl
						//Read Arguments
						//Expect(lexer, executionContext, EToken::SeparatorRBracketRound)
						//BuildScopedInstructionSequence
						break;
					}

					case EToken::OperatorAssign:
					{
						lexer.ReadNextToken();

						VariableDeclarationNode* variableDeclaration{ new VariableDeclarationNode{} };
						variableDeclaration->SetVariableName(indentifierToken.TokenText);
						variableDeclaration->SetVariableType(indentifierToken.TokenType);
						variableDeclaration->SetInitExpression(BuildExpression(lexer, executionContext));
						Expect(lexer, executionContext, EToken::SeparatorInstructionEnd);
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

		Node* BuildExpression(Lexer& lexer, ExecutionContext& executionContext)
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
							tokenStack.push(BuildFunctionCall(lexer, executionContext, currentToken));
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

		Node* BuildFunctionCall(Lexer& lexer, ExecutionContext& executionContext, const Token& calledFunction)
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
					functionCallNode->AddArgument(BuildExpression(lexer, executionContext));
					Token followingToken{ lexer.PeekNextToken() };
					if (followingToken.TokenType != EToken::SeparatorRBracketRound && followingToken.TokenType != EToken::SeparatorComma)
					{
						//TODO: error
					}
				}
			}
			return functionCallNode;
		}

		Node* BuildLoop(Lexer& lexer, ExecutionContext& executionContext)
		{
			return nullptr;
		}

		Node* BuildScopedInstructionSequence(Lexer& lexer, ExecutionContext& executionContext)
		{
			InstructionSequenceNode* instructionSequence{ new InstructionSequenceNode{} };
			while (lexer.PeekNextToken().TokenType != EToken::SeparatorRBracketCurly)
			{
				if (Node* builtNode = Internal::BuildInstruction(lexer, executionContext))
				{
					instructionSequence->AddInstruction(builtNode);
				}
				else
				{
					// TODO: error
					break;
				}
			}
			return instructionSequence;
		}
	}

	Node* BuildAST(Lexer& lexer, ExecutionContext& executionContext)
	{
		InstructionSequenceNode* instructionSequence{ new InstructionSequenceNode{} };

		while (lexer.HasUnreadText()) // TODO: break if an error occurs
		{
			if (Node* builtNode = Internal::BuildInstruction(lexer, executionContext))
			{
				instructionSequence->AddInstruction(builtNode);
			}
		}

		return instructionSequence;
	}
}