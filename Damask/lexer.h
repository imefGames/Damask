#pragma once

#include <string_view>
#include <vector>

enum class EToken
{
	Identifier,

	//Keywords
	KeywordIf,
	KeywordElse,
	KeywordWhile,
	KeywordFor,
	KeywordDo,

	//Type Names
	TypeNameVoid,
	TypeNameS32,
	TypeNameU32,

	//Separators
	SeparatorLBracketRound,
	SeparatorRBracketRound,
	SeparatorLBracketSquare,
	SeparatorRBracketSquare,
	SeparatorLBracketCurly,
	SeparatorRBracketCurly,
	SeparatorInstructionEnd,
	SeparatorComma,

	//Operator
	OperatorAdd,
	OperatorSub,
	OperatorMul,
	OperatorDiv,
	OperatorMod,
	OperatorAssign,

	//Literals
	LiteralBool,
	LiteralChar,
	LiteralNumber,
	LiteralString,

	//Other
	Comment,
	Unknown,
	None
};

struct Token
{
	Token();
	Token(std::string_view tokenText, EToken tokenType);

	std::string_view TokenText;
	EToken TokenType;
};

struct Lexer
{
public:
	Lexer(std::string_view buffer);

	void RegisterReservedToken(const std::string_view& tokenText, EToken tokenType);

	bool HasUnreadText() const;

	Token ReadNextToken();
	Token& PeekNextToken();

private:
	Token BuildNextToken();
	EToken GetTokenType(std::string_view tokenText) const;
	bool IsSeparator(char c) const;

	Token m_NextToken;
	std::vector<Token> m_ReservedTokens;
	std::string_view m_Buffer;
};