#include "codegen.hpp"
#include "ast.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

namespace CustomLang {

    CodeGenerator::CodeGenerator() {
        context_ = std::make_unique<llvm::LLVMContext>();
        module_ = std::make_unique<llvm::Module>("CustomLang", *context_);
        builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);
        currentFunction_ = nullptr;
        khaliValue_ = nullptr;

        // Create print function declaration
        createPrintFunction();

        // Create khali constant
        createKhaliConstant();
    }
    
    CodeGenerator::~CodeGenerator() = default;

    std::unique_ptr<llvm::Module> CodeGenerator::generateIR(
        const std::vector<std::unique_ptr<Statement>>& ast) {

        // Process each top-level node
        for (const auto& node : ast) {
            node->accept(*this);
        }

        return std::move(module_);
    }

    void CodeGenerator::createKhaliConstant() {
        // Create a null pointer constant that we'll use for 'khali'
        khaliValue_ = llvm::ConstantPointerNull::get(
            llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0));
    }

    void CodeGenerator::visitLiteralExpr(LiteralExpr& expr) {
        llvm::Value* value = nullptr;

        switch (expr.type) {
        case LiteralExpr::LiteralType::NUMBER:
            value = llvm::ConstantInt::get(*context_,
                llvm::APInt(32, std::stoi(expr.value), true));
            break;

        case LiteralExpr::LiteralType::STRING:
            value = createStringConstant(expr.value);
            break;

        case LiteralExpr::LiteralType::BOOLEAN:
            value = llvm::ConstantInt::get(*context_,
                llvm::APInt(1, expr.value == "true" ? 1 : 0));
            break;

        case LiteralExpr::LiteralType::KHALI:
            value = khaliValue_;
            break;
        }

        // Register with garbage collector if it's a heap-allocated value
        if (value && !llvm::isa<llvm::Constant>(value)) {
            registerWithGC(value);
        }
    }

    void CodeGenerator::registerWithGC(llvm::Value* value) {
        // Create call to GC registration function
        std::vector<llvm::Type*> registerArgs = {
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context_))
        };

        llvm::FunctionType* registerType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(*context_),
            registerArgs,
            false);

        llvm::Function* registerFunc = module_->getFunction("GC_register");
        if (!registerFunc) {
            registerFunc = llvm::Function::Create(
                registerType,
                llvm::Function::ExternalLinkage,
                "GC_register",
                module_.get());
        }

        builder_->CreateCall(registerFunc, {
            builder_->CreateBitCast(value, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context_)))
            });
    }


    void CodeGenerator::visitPrintStatement(PrintStatement& stmt) {
        stmt.expression->accept(*this);
        // Call print function here
    }

    void CodeGenerator::visitFunctionDecl(FunctionDecl& decl) {
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : decl.params) {
        if (param.type == "int") {
            paramTypes.push_back(llvm::Type::getInt32Ty(*context_));
        }
        else if (param.type == "string") {
            paramTypes.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0));
        }
        // Add other types as needed
    }

    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context_), paramTypes, false);

    llvm::Function* function = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, decl.name, module_.get());

    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(
        *context_, "entry", function);
    builder_->SetInsertPoint(entryBlock);

    // Generate code for function body
    for (const auto& node : decl.body) {
        node->accept(*this);
    }
}


    void CodeGenerator::createPrintFunction() {
    std::vector<llvm::Type*> printArgs;
    printArgs.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0));

    llvm::FunctionType* printType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context_), printArgs, false);

    module_->getOrInsertFunction("dikha_bhai", printType);
}


    llvm::Value* CodeGenerator::createStringConstant(const std::string& str) {
        return builder_->CreateGlobalStringPtr(str);
    }

} // namespace CustomLang