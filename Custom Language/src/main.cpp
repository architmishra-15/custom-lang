#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include "runtime.hpp"
#include "colors.hpp"
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <cstdlib>
#include "ast.hpp"

bool hasValidExtension(const std::string& filename) {
    std::filesystem::path path(filename);
    std::string ext = path.extension().string();
    return ext == ".awara" || ext == ".aw";
}

// Global logError function
void logError(const std::string& baseMessage, const std::string& context = "", const std::string& suggestion = "") {
    std::ostringstream oss;
    oss << colorize("Bhai nhi chal paya! Error aa gaya: ", RED) << baseMessage;
    
    if (!context.empty()) {
        oss << "\n" << colorize("Ye chhod diya: ", CYAN) << context;
    }

    if (!suggestion.empty()) {
        oss << "\n" << colorize("Socho iske baare mein: ", GREEN) << suggestion;
    }

    std::cerr << oss.str() << "\n" << std::endl;
    throw std::runtime_error(oss.str());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        logError("Usage: " + std::string(argv[0]) + " <source_file.awara> [--emit-llvm] [--verbose] [--output=<binary_name>]",
                "No input file provided",
                "Please provide a source file with .awara or .aw extension");
        return 1;
    }

    std::string sourceFile = argv[1];
    bool emitLLVM = false;
    bool verbose = false;
    std::string outputBinary = "output";

    // Parse optional arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--emit-llvm") {
            emitLLVM = true;
        } else if (arg == "--verbose") {
            verbose = true;
        } else if (arg.find("--output=") == 0) {
            outputBinary = arg.substr(9);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 2;
        }
    }

    if (!hasValidExtension(sourceFile)) {
        logError("File must have .awara or .aw extension",
                "Provided file: " + sourceFile,
                "Rename your file to end with .awara or .aw");
        return 3;
    }

    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    try {
        // Read source file
        std::ifstream file(sourceFile);
        if (!file.is_open()) {
            logError("Could not open file",
                    "File: " + sourceFile,
                    "Check if the file exists and you have permission to read it");
            return 4;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();

        if (source.empty()) {
            logError("Source file is empty",
                    "File: " + sourceFile,
                    "Add some code to your source file");
            return 5;
        }

        // Initialize runtime
        CustomLang::Runtime::initialize();

        // Create lexer and parser
        CustomLang::Lexer lexer(source);
        CustomLang::Parser parser(lexer);

        // Parse source code
        std::vector<std::unique_ptr<CustomLang::Statement>> ast;
        try {
            ast = parser.parse();
            if (verbose) {
                std::cout << "Parsing completed successfully." << std::endl;
            }
        } catch (const std::exception& e) {
            logError("Parsing Error",
                    e.what(),
                    "Check your syntax and try again");
            return 6;
        }

        // Generate LLVM IR
        CustomLang::CodeGenerator codegen;
        std::unique_ptr<llvm::Module> module;
        try {
            module = codegen.generateIR(ast); // Changed to accept vector of statements
            if (verbose) {
                std::cout << "Code generation completed successfully." << std::endl;
            }
        } catch (const std::exception& e) {
            logError("Code Generation Error",
                    e.what(),
                    "Check your semantic rules and try again");
            return 7;
        }

        if (emitLLVM) {
            // Emit LLVM IR to a file
            std::string llvmIRFilename = outputBinary + ".ll";
            std::error_code ec;
            llvm::raw_fd_ostream llvmIRFile(llvmIRFilename, ec);
            if (ec) {
                logError("Could not write LLVM IR to file",
                        llvmIRFilename + " (" + ec.message() + ")",
                        "Check your write permissions and available disk space");
                return 8;
            }
            module->print(llvmIRFile, nullptr);
            std::cout << "LLVM IR emitted to " << llvmIRFilename << std::endl;
        } else {
            // Execute or compile binary (placeholder for further logic)
            std::cout << "Compilation or execution of binary would proceed here." << std::endl;
        }

        return 0;
    } catch (const std::exception& e) {
        logError("Unexpected Error",
                e.what(),
                "Please report this issue if it persists");
        return 9;
    }
}