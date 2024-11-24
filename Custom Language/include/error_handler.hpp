#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <string>
#include <vector>
#include <iostream>

// ANSI Color Codes
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"

namespace CustomLang {

class ErrorHandler {
public:
    struct Error {
        int line;
        int column;
        std::string message;
        std::string hint;
    };

    void addError(int line, int column, const std::string& message, const std::string& hint = "") {
        errors_.push_back({line, column, message, hint});
    }

    void displayErrors(bool verbose = false) const {
        for (const auto& error : errors_) {
            std::cout << COLOR_RED << "[Error]" << COLOR_RESET
                      << " Line " << error.line << ", Column " << error.column << ": "
                      << error.message << "\n";

            if (!error.hint.empty()) {
                std::cout << COLOR_GREEN << "Hint: " << error.hint << COLOR_RESET << "\n";
            }

            if (verbose) {
                std::cout << COLOR_YELLOW
                          << "Verbose: Ensure your syntax matches the language specifications."
                          << COLOR_RESET << "\n";
            }

            std::cout << "\n"; // Blank line between errors
        }

        if (errors_.empty()) {
            std::cout << COLOR_CYAN << "No errors found. All good!" << COLOR_RESET << "\n";
        }
    }

    bool hasErrors() const {
        return !errors_.empty();
    }

private:
    std::vector<Error> errors_;
};

} // namespace CustomLang

#endif
