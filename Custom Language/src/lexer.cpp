#include "lexer.hpp"
#include <stdexcept>
#include <sstream>

namespace CustomLang {

    char Lexer::advance() {
        current_++;
        column_++;
        return source_[current_ - 1];
    }

    char Lexer::peek() const {
        if (isAtEnd()) return '\0';
        return source_[current_];
    }

    char Lexer::peekNext() const {
        if (current_ + 1 >= source_.length()) return '\0';
        return source_[current_ + 1];
    }

    bool Lexer::isAtEnd() const {
        return current_ >= source_.length();
    }

    bool Lexer::match(char expected) {
        if (isAtEnd() || source_[current_] != expected) return false;
        current_++;
        column_++;
        return true;
    }

    Token Lexer::makeToken(TokenType type) {
        std::string text = source_.substr(start_, current_ - start_);
        Token token(type, text, line_, column_ - text.length());
        return token;
    }

    Token Lexer::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') {
                line_++;
                column_ = 1;
            }
            advance();
        }

        if (isAtEnd()) {
            throw std::runtime_error("Unterminated string at line " +
                std::to_string(line_));
        }

        // Consume the closing "
        advance();

        // Get the string content (without the quotes)
        std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
        return Token(TokenType::STRING_LITERAL, value, line_, column_);
    }

    Token Lexer::number() {
        bool isFloat = false;

        while (isdigit(peek())) advance();

        // Look for decimal part
        if (peek() == '.' && isdigit(peekNext())) {
            isFloat = true;
            advance(); // Consume the .
            while (isdigit(peek())) advance();
        }

        return makeToken(isFloat ? TokenType::FLOAT_LITERAL : TokenType::NUMBER_LITERAL);
    }

    Token Lexer::nextToken() {
        skipWhitespace();

        start_ = current_;

        if (isAtEnd()) return makeToken(TokenType::EOF_TOKEN);

        char c = advance();

        // Handle identifiers and keywords
        if (isalpha(c) || c == '_') {
            while (isalnum(peek()) || peek() == '_' || peek() == '.') advance();

            std::string text = source_.substr(start_, current_ - start_);

            // Check if it's a keyword
            auto it = keywords_.find(text);
            if (it != keywords_.end()) {
                return makeToken(it->second);
            }

            return makeToken(TokenType::IDENTIFIER);
        }

        // Handle numbers
        if (isdigit(c)) {
            return number();
        }

        // Handle strings
        if (c == '"') {
            return string();
        }

        // Handle other tokens
        switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ',': return makeToken(TokenType::COMMA);
            // Add other single-character tokens
        }

        std::stringstream error;
        error << "Unexpected character at line " << line_ << ", column " << column_;
        throw std::runtime_error(error.str());
    }

    void Lexer::skipWhitespace() {
        while (true) {
            char c = peek();
            switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line_++;
                column_ = 1;
                advance();
                break;
            default:
                return;
            }
        }
    }

} // namespace CustomLang