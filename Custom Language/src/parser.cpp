#include "parser.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include "colors.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>

namespace CustomLang {

// Advance to the next token
void Parser::advance() {
    current_token_ = lexer_.nextToken();
}

// Match a token type and advance if matched
bool Parser::match(TokenType type) {
    if (current_token_.type == type) {
        advance();
        return true;
    }
    return false;
}

// Centralized error handler
void Parser::logError(const std::string& baseMessage, const std::string& context, const std::string& suggestion) {
    std::ostringstream oss;
    oss << colorize("Bhai nhi chal paya! Error aa gaya: ", RED) << baseMessage
        << "\n" << colorize("Ye chhod diya: ", CYAN) << context;

    if (!suggestion.empty()) {
        oss << "\n" << colorize("Socho iske baare mein: ", GREEN) << suggestion;
    }
    std::cerr << oss.str() << "\n" << std::endl;
    throw std::runtime_error(oss.str());
}

// Expect a specific token type, or add an error if it doesn't match
void Parser::expect(TokenType type, const std::string& errorMessage) {
    if (!match(type)) {
        logError(colorize(errorMessage, RED),
                colorize(current_token_.lexeme, GREEN),
                colorize("Dekho syntax sahi hai kya?", CYAN));
    }
}

// Parse a single statement
std::unique_ptr<Statement> Parser::parseStatement() {
    switch (current_token_.type) {
        case TokenType::DIKHA_BHAI:
            return parsePrintStatement();
        case TokenType::DEKH:
            return parseFunctionDeclaration();
        default:
            logError(colorize("Ooo Bhai kaha? Kya chala rha h? ", RED),
                    colorize(current_token_.lexeme, GREEN));
            return nullptr;
    }
}


// Parse a print statement
std::unique_ptr<Statement> Parser::parsePrintStatement() {
    expect( TokenType::DIKHA_BHAI,
            colorize("Kya bhai shi se bta to dikhane ko - ", RED) + colorize("'dikha bhai'", MAGENTA));
    auto expression = parseExpression();
    return std::make_unique<PrintStatement>(std::move(expression));
}

// Parse a function declaration
std::unique_ptr<Statement> Parser::parseFunctionDeclaration() {
    expect( TokenType::DEKH,
            colorize("Bhai agar kuch bta rhe ho to bta to do, ", RED) +
            colorize("'dekh'", MAGENTA)
        );

    // Parse function name
    if (current_token_.type == TokenType::IDENTIFIER) {
    std::string functionName = current_token_.lexeme;
    advance();

    // Parse parameters
    expect( TokenType::LEFT_PAREN, 
            colorize("Kya be ", BOLD_RED) +
            colorize("'(' ", BOLD_GREEN) +
            colorize ("ke dali ho?", BOLD_RED)
        );

    std::vector<FunctionDecl::Param> params;

    while (!match(TokenType::RIGHT_PAREN)) {

        if (!params.empty()) {
            expect( TokenType::COMMA,
                    colorize("Tsk, tsk comma lga da ho marde - ", RED) +
                    colorize(",", MAGENTA)
                );
        }

        expect(TokenType::IDENTIFIER, colorize("Parameter name to daal!", BOLD_RED));
        std::string paramName = current_token_.lexeme;
        advance();
        expect(TokenType::IDENTIFIER, colorize("Parameter name to daal!", BOLD_RED));
        std::string paramType = current_token_.lexeme;
        params.emplace_back(FunctionDecl::Param{paramName, paramType});
    }

    // Parse function body
    expect( TokenType::LEFT_BRACE,
            colorize("'{'", BOLD_MAGENTA) +
            colorize(" ye to lga do", BLUE)
        );
    std::vector<std::unique_ptr<Statement>> body;
    while (!match(TokenType::RIGHT_BRACE)) {
        body.push_back(parseStatement());
    }

    return std::make_unique<FunctionDecl>(functionName, params, std::move(body));
    } 
    
   else {
        logError(
            colorize("Kya bta rhe ho? Naam to define kro! ", BOLD_CYAN) +
            current_token_.lexeme, 
            colorize("'dekh' ", RED) +
            colorize("ke baad naam hona chahiye.", BOLD_CYAN)
        );
        return nullptr;
    }
}

// Parse an expression
std::unique_ptr<Expression> Parser::parseExpression() {
    return parseBinaryExpression();
}

// Parse a binary expression
std::unique_ptr<Expression> Parser::parseBinaryExpression(int precedence) {
    auto left = parsePrimary();

    while (true) {
        int tokenPrecedence = getPrecedence(current_token_.type);
        if (tokenPrecedence < precedence) {
            break;
        }

        std::string op = current_token_.lexeme;
        advance();

        auto right = parseBinaryExpression(tokenPrecedence + 1);
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

// Parse a primary expression
std::unique_ptr<Expression> Parser::parsePrimary() {
    switch (current_token_.type) {
        case TokenType::NUMBER_LITERAL:
        case TokenType::STRING_LITERAL:
        case TokenType::BOOLEAN_LITERAL:
        case TokenType::KHALI:
            return parseLiteral();
        default:
            logError(colorize("Kuch to hua: ", GREEN), current_token_.lexeme);
            return nullptr;
    }
}

// Parse a literal expression
std::unique_ptr<Expression> Parser::parseLiteral() {
    std::string value = current_token_.lexeme;
    TokenType type = current_token_.type;

    advance(); // Consume the literal token

    if (type == TokenType::NUMBER_LITERAL) {
        return std::make_unique<LiteralExpr>(LiteralExpr::LiteralType::NUMBER, value);
    } else if (type == TokenType::STRING_LITERAL) {
        return std::make_unique<LiteralExpr>(LiteralExpr::LiteralType::STRING, value);
    } else if (type == TokenType::BOOLEAN_LITERAL) {
        return std::make_unique<LiteralExpr>(LiteralExpr::LiteralType::BOOLEAN, value);
    } else if (type == TokenType::KHALI) {
        return std::make_unique<LiteralExpr>(LiteralExpr::LiteralType::KHALI, "khali");
    } else {
        logError(colorize("Kya kr rha h bhai tu? Syntax to dhang se daalna seekh le", RED), value);
        return nullptr;
    }
}

// Get operator precedence
int Parser::getPrecedence(TokenType type) const {
    switch (type) {
        case TokenType::AUR:
        case TokenType::YA:
            return 1; // Low precedence
        default:
            return 0;
    }
}

} // namespace CustomLang
