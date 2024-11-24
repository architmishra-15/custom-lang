#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>

namespace CustomLang {
    class Parser {
    public:
        explicit Parser(Lexer lexer) 
            : lexer_(std::move(lexer)), 
              current_token_(TokenType::INVALID, "", 0, 0) {
            advance(); // Load first token
        }

        std::vector<std::unique_ptr<Statement>> parse();

    private:
        Lexer lexer_;
        Token current_token_;

        void advance();
        bool match(TokenType type);
        bool check(TokenType type);
        Token consume(TokenType type, const std::string& message);

        // Parsing methods
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Statement> parsePrintStatement();
        std::unique_ptr<Statement> parseFunctionDeclaration();
        std::unique_ptr<Expression> parseExpression();
        std::unique_ptr<Expression> parseBinaryExpression(int precedence = 0);
        
        // Helper methods for parsing expressions
        std::unique_ptr<Expression> parsePrimary();
        std::unique_ptr<Expression> parseLiteral();
        std::unique_ptr<Expression> parseGrouping();
        
        // Error handling
        void logError(const std::string& baseMessage, 
                     const std::string& context, 
                     const std::string& suggestion = "");
        void expect(TokenType type, const std::string& errorMessage);
        void synchronize(); // Error recovery
        std::unique_ptr<Expression> error(const std::string& message);
        std::unique_ptr<Statement> errorStmt(const std::string& message);
        
        // Helper methods
        int getPrecedence(TokenType type) const;
        
        // Function parsing helpers
        std::vector<std::pair<std::string, TokenType>> parseParameters();
        std::vector<std::unique_ptr<Statement>> parseBlock();
    };
}