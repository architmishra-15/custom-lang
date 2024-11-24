// ast.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>

namespace CustomLang {

    // Forward declarations
    class ASTVisitor;

    // Base AST Node
    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual void accept(ASTVisitor& visitor) = 0;
    };

    // Expression nodes
    class Expression : public ASTNode {
    public:
        virtual ~Expression() = default;
    };

    // Statement nodes
    class Statement : public ASTNode {
    public:
        virtual ~Statement() = default;
    };

    // Literal expression (numbers, strings, etc.)
    class LiteralExpr : public Expression {
    public:
        enum class LiteralType {
            NUMBER,
            STRING,
            BOOLEAN,
            KHALI
        };

        LiteralType type;
        std::string value;

        LiteralExpr(LiteralType t, std::string v)
            : type(t), value(std::move(v)) {}

        void accept(ASTVisitor& visitor) override;
    };

    // Binary expression (aur, ya)
    class BinaryExpr : public Expression {
    public:
        std::unique_ptr<Expression> left;
        std::string op;
        std::unique_ptr<Expression> right;

        BinaryExpr(std::unique_ptr<Expression> l, std::string o, std::unique_ptr<Expression> r)
            : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}

        void accept(ASTVisitor& visitor) override;
    };

    // Print Statement
    class PrintStatement : public Statement {
    public:
        std::unique_ptr<Expression> expression;

        explicit PrintStatement(std::unique_ptr<Expression> expr)
            : expression(std::move(expr)) {}

        void accept(ASTVisitor& visitor) override;
    };

    // Function declaration
    class FunctionDecl : public Statement {
    public:
        struct Param {
            std::string name;
            std::string type;
        };

        std::string name;
        std::vector<Param> params;
        std::vector<std::unique_ptr<Statement>> body;
        std::unique_ptr<Expression> returnExpr;

        FunctionDecl(std::string n, std::vector<Param> p, std::vector<std::unique_ptr<Statement>> b)
        : name(std::move(n)), params(std::move(p)), body(std::move(b)) {}

    void accept(ASTVisitor& visitor) override;
    };

    // Visitor pattern for traversing AST
    class ASTVisitor {
    public:
        virtual ~ASTVisitor() = default;
        virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
        virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
        virtual void visitPrintStatement(PrintStatement& stmt) = 0;
        virtual void visitFunctionDecl(FunctionDecl& decl) = 0;
    };

} // namespace CustomLang