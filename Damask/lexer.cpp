#include <lexer.h>

#include <algorithm>
#include <cctype>

Token::Token()
	: TokenType{ EToken::None }
{
}

Token::Token(std::string_view tokenText, EToken tokenType)
	: TokenText{ tokenText }
	, TokenType{ tokenType }
{
}



Lexer::Lexer(std::string_view buffer)
	: m_Buffer{ buffer }
{
	RegisterReservedToken("(", EToken::SeparatorLBracketRound);
	RegisterReservedToken(")", EToken::SeparatorRBracketRound);
	RegisterReservedToken("[", EToken::SeparatorLBracketSquare);
	RegisterReservedToken("]", EToken::SeparatorRBracketSquare);
	RegisterReservedToken("{", EToken::SeparatorLBracketCurly);
	RegisterReservedToken("}", EToken::SeparatorRBracketCurly);
	RegisterReservedToken(";", EToken::SeparatorInstructionEnd);
	RegisterReservedToken(",", EToken::SeparatorComma);

	RegisterReservedToken("+", EToken::OperatorAdd);
	RegisterReservedToken("-", EToken::OperatorSub);
	RegisterReservedToken("*", EToken::OperatorMul);
	RegisterReservedToken("/", EToken::OperatorDiv);
	RegisterReservedToken("%", EToken::OperatorMod);
	RegisterReservedToken("=", EToken::OperatorAssign);

	RegisterReservedToken("do", EToken::KeywordDo);
	RegisterReservedToken("if", EToken::KeywordIf);
	RegisterReservedToken("for", EToken::KeywordFor);
	RegisterReservedToken("while", EToken::KeywordWhile);
	RegisterReservedToken("else", EToken::KeywordElse);

	RegisterReservedToken("true", EToken::LiteralBool);
	RegisterReservedToken("false", EToken::LiteralBool);

	RegisterReservedToken("void", EToken::TypeNameVoid);
	RegisterReservedToken("s32", EToken::TypeNameS32);
	RegisterReservedToken("u32", EToken::TypeNameU32);
}

void Lexer::RegisterReservedToken(const std::string_view& tokenText, EToken tokenType)
{
	m_ReservedTokens.emplace_back(tokenText, tokenType);
}

bool Lexer::HasUnreadText() const
{
	return !m_Buffer.empty();
}

Token Lexer::ReadNextToken()
{
	Token nextToken;
	if (m_NextToken.TokenType == EToken::None)
	{
		nextToken = BuildNextToken();
	}
	else
	{
		nextToken = m_NextToken;
		m_NextToken.TokenType = EToken::None;
	}
	return nextToken;
}

Token& Lexer::PeekNextToken()
{
	if (m_NextToken.TokenType == EToken::None)
	{
		m_NextToken = BuildNextToken();
	}
	return m_NextToken;
}

Token Lexer::BuildNextToken()
{
	// TODO: Look for invalid characters (not printable ?)
	// TODO: handle comments

	m_Buffer.remove_prefix(std::min(m_Buffer.find_first_not_of("\t\n\v\f\r "), m_Buffer.size()));

	Token token;
	if (!m_Buffer.empty())
	{
		unsigned int tokenEnd{ 1 };
		char firstChar{ m_Buffer[0] };
		if (std::isdigit(firstChar))
		{
			while (std::isdigit(m_Buffer[tokenEnd]))
			{
				++tokenEnd;
			}
			if (m_Buffer[tokenEnd] == '.')
			{
				++tokenEnd;
				while (std::isdigit(m_Buffer[tokenEnd]))
				{
					++tokenEnd;
				}
			}

			token.TokenText = std::string_view{ &m_Buffer[0], tokenEnd };
			token.TokenType = EToken::LiteralNumber;
		}
		else if (firstChar == '"')
		{
			while (m_Buffer[tokenEnd] != '"')
			{
				++tokenEnd;
			}
			++tokenEnd;

			token.TokenText = std::string_view{ &m_Buffer[0], tokenEnd };
			token.TokenType = EToken::LiteralString;
		}
		else if (firstChar == '\'')
		{
			// TODO
			token.TokenType = EToken::LiteralChar;
		}
		else if (std::isalpha(firstChar))
		{
			while (std::isalnum(m_Buffer[tokenEnd]))
			{
				++tokenEnd;
			}

			token.TokenText = std::string_view{ &m_Buffer[0], tokenEnd };
			token.TokenType = GetTokenType(token.TokenText);
		}
		else if (IsSeparator(firstChar))
		{
			token.TokenText = std::string_view{ &m_Buffer[0], tokenEnd };
			token.TokenType = GetTokenType(token.TokenText);
		}
		else
		{
			while (std::ispunct(m_Buffer[tokenEnd]))
			{
				++tokenEnd;
			}
			token.TokenText = std::string_view{ &m_Buffer[0], tokenEnd };
			token.TokenType = GetTokenType(token.TokenText);
		}
		m_Buffer.remove_prefix(tokenEnd);
	}
	return token;
}

EToken Lexer::GetTokenType(std::string_view tokenText) const
{
	auto foundIt{ std::find_if(m_ReservedTokens.begin(), m_ReservedTokens.end(),
		[&tokenText](const Token& token) { return token.TokenText == tokenText; }) };
	return (foundIt != m_ReservedTokens.end() ? foundIt->TokenType : EToken::Identifier);
}

bool Lexer::IsSeparator(char c) const
{
	static const char separators[]{ "()[]{};" };
	return (std::find(std::begin(separators), std::end(separators), c) != std::end(separators));
}