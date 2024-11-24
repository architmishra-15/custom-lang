# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/codegen.cpp
    src/runtime.cpp
    src/main.cpp
    src/gc.cpp
)

# Link against the appropriate LLVM libraries
# Define llvm_libs based on the components you need
set(llvm_libs
    LLVMCore
    LLVMSupport
    LLVMIRReader
    LLVMAnalysis
)

# Create executable
add_executable(custom_lang ${SOURCES})

# Link against LLVM libraries
target_link_libraries(custom_lang /usr/lib/libLLVM-18.so)

# Add tests directory
# add_subdirectory(tests)