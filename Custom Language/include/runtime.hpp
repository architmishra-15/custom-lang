#pragma once
#include <string>
#include "gc.hpp"

namespace CustomLang {

    class Runtime {
    public:
        // Initialize runtime environment
        static void initialize();

        // Print function implementation
        static void print(const std::string& message);

        // Runtime type checking
        static bool checkType(const std::string& value, const std::string& expectedType);

        // Memory management functions
        static void* allocateMemory(size_t size);
        static void freeMemory(void* ptr);
        static void markRoot(void* ptr);
        static void collectGarbage();
        static bool isKhali(void* ptr);
        static void* createKhali();

        // Error handling
        static void handleError(const std::string& message);
    };

} // namespace CustomLang