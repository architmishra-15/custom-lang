// ast.cpp
#include "ast.hpp"

namespace CustomLang {

    // Implement accept methods for all AST nodes
    void LiteralExpr::accept(ASTVisitor& visitor) {
        visitor.visitLiteralExpr(*this);
    }

    void BinaryExpr::accept(ASTVisitor& visitor) {
        visitor.visitBinaryExpr(*this);
    }

    void PrintStatement::accept(ASTVisitor& visitor) {
        visitor.visitPrintStatement(*this);
    }

    void FunctionDecl::accept(ASTVisitor& visitor) {
        visitor.visitFunctionDecl(*this);
    }

} // namespace CustomLang