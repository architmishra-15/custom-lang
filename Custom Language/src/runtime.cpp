#include "runtime.hpp"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <unordered_set>

namespace CustomLang {
    
    static GarbageCollector& gc = GarbageCollector::getInstance();

    void Runtime::initialize() {
        
        gc = GarbageCollector::getInstance();

    }

    void Runtime::print(const std::string& message) {
        std::cout << message << std::endl;
    }

    bool Runtime::checkType(const std::string& value, const std::string& expectedType) {
        if (expectedType == "int") {
            try {
                std::stoi(value);
                return true;
            }
            catch (...) {
                return false;
            }
        }
        else if (expectedType == "float") {
            try {
                std::stof(value);
                return true;
            }
            catch (...) {
                return false;
            }
        }
        // Add more type checks as needed
        return true;
    }

    void* Runtime::allocateMemory(size_t size) {
        void* ptr = gc.allocate(size);
        if (!ptr) {
            throw std::runtime_error("Memory allocation failed");
        }
        return ptr;
    }

    void Runtime::freeMemory(void* ptr) {
        gc.removeRoot(ptr);
    }

    void Runtime::markRoot(void* ptr) {
        gc.addRoot(ptr);
    }

    void Runtime::collectGarbage() {
        gc.collect();
    }

    bool Runtime::isKhali(void* ptr) {
        return ptr == nullptr;
    }

    void* Runtime::createKhali() {
        return nullptr;
    }

    void Runtime::handleError(const std::string& message) {
        std::cerr << "Runtime Error: " << message << std::endl;
        throw std::runtime_error(message);
    }

} // namespace CustomLang