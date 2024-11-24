#pragma once
#include "ast.hpp"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <map>
#include <memory>

namespace CustomLang {
    class CodeGenerator : public ASTVisitor {
    public:
        CodeGenerator();
        ~CodeGenerator() override;  // Added override since we inherit from ASTVisitor

        // Main code generation method
        std::unique_ptr<llvm::Module> generateIR(const std::vector<std::unique_ptr<Statement>>& ast);

        // Visitor pattern implementation
        void visitLiteralExpr(LiteralExpr& expr) override;
        void visitBinaryExpr(BinaryExpr& expr) override;
        void visitPrintStatement(PrintStatement& stmt) override;
        void visitFunctionDecl(FunctionDecl& decl) override;

    private:
        // LLVM infrastructure
        std::unique_ptr<llvm::LLVMContext> context_;
        std::unique_ptr<llvm::Module> module_;
        std::unique_ptr<llvm::IRBuilder<>> builder_;

        // Last generated value (for expression evaluation)
        llvm::Value* lastValue_{nullptr};  // Added to store expression results

        // Symbol table for variables and functions
        std::map<std::string, llvm::Value*> symbolTable_;

        // Current function being generated
        llvm::Function* currentFunction_{nullptr};  // Added initialization

        // Khali value constant
        llvm::Value* khaliValue_{nullptr};  // Added initialization

        // Helper methods
        llvm::Value* createStringConstant(const std::string& str);
        void createPrintFunction();
        void createKhaliConstant();
        void registerWithGC(llvm::Value* value);

        // Added helper to get the last generated value
        llvm::Value* getLastValue() { return lastValue_; }
    };

} // namespace CustomLang