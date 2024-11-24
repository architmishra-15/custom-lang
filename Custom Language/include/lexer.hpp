#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace CustomLang {

	enum class TokenType {
		// Keywords
		DIKHA_BHAI,		// print
		DEKH,			// declaring function
		WAPAS_KRO,		// return
		AUR,			// and
		YA,				// or
		KHALI,			// None/null

		// Data types
		INT,
		STRING,
		FLOAT,
		BOOL,

		// Symbols
		LEFT_PAREN,    // (
		RIGHT_PAREN,   // )
		LEFT_BRACE,    // {
		RIGHT_BRACE,   // }
		COMMA,         // ,

		// Literals and Identifiers
		IDENTIFIER,
		STRING_LITERAL,
		NUMBER_LITERAL,
		FLOAT_LITERAL,
		BOOLEAN_LITERAL,

		// Comments
		SINGLE_LINE_COMMENT,    // //
		BLOCK_COMMENT,         // mt-padh!

		// Other
		EOF_TOKEN,
		INVALID
	};

	class Token {
	public:
		TokenType type;
		std::string lexeme;
		int line;
		int column;

		Token(TokenType t, std::string l, int ln, int col)
			:type(t), lexeme(std::move(l)), line(ln), column(col) {}

	};

	class Lexer {
	public:
		explicit Lexer(std::string source) : source_(std::move(source)) {
			initializedKeywords();
		}

		Token nextToken();

	private:
		std::string source_;
		size_t current_ = 0;
		size_t start_ = 0;
		int line_ = 1;
		int column_ = 1;

		std::unordered_map<std::string, TokenType> keywords_;

		void initializedKeywords() {
			keywords_["dikha"] = TokenType::DIKHA_BHAI;
			keywords_["bhai"] = TokenType::DIKHA_BHAI;
			keywords_["dekh"] = TokenType::DEKH;
			keywords_["wapas.kro"] = TokenType::WAPAS_KRO;
			keywords_["aur"] = TokenType::AUR;
			keywords_["ya"] = TokenType::YA;
			keywords_["khali"] = TokenType::KHALI;
			keywords_["int"] = TokenType::INT;
			keywords_["string"] = TokenType::STRING;
			keywords_["float"] = TokenType::FLOAT;
			keywords_["bool"] = TokenType::BOOL;

		}

		char advance();
		char peek() const;
		char peekNext() const;
		bool isAtEnd() const;
		bool match(char expected);

		Token makeToken(TokenType type);
		Token string();
		Token number();
		Token identifier();
		void skipWhitespace();
		void skipComment();
		void skipBlockComment();

	};
}