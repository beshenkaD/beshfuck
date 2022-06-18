cmake_minimum_required(VERSION 3.5)

project(beshfuck)

set(CMAKE_C_STANDARD 99)

set(SOURCES
    src/main.c
    src/vm.c
    src/vm_bytecode.c
    src/vm_debug.c
    src/vm_constants.c
    src/vm_procedure.c
    src/compiler.c
    src/map.c
    src/interpreter.c
    src/lexer.c
    src/interpreter_repl.c
)

add_executable(beshfuck ${SOURCES})

target_compile_options(beshfuck 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic 
    $<$<CONFIG:DEBUG>: -Werror>
    $<$<CONFIG:DEBUG>: -Wno-unused-function>
)

option(DEBUG_PRINT "Print disassembled bytecode after compilation" OFF)
option(ENABLE_BOUNDS_CHECK "Enable or disable beshfuck bound-checking" ON)
option(USE_LIBEDIT "Use libedit for REPL" ON)

if(DEBUG_PRINT)
    target_compile_definitions(beshfuck PRIVATE DEBUG_PRINT)
endif(DEBUG_PRINT)

if(ENABLE_BOUNDS_CHECK)
    target_compile_definitions(beshfuck PRIVATE ENABLE_BOUNDS_CHECK)
endif(ENABLE_BOUNDS_CHECK)

if(USE_LIBEDIT)
    target_compile_definitions(beshfuck PRIVATE USE_LIBEDIT)
    target_link_libraries(beshfuck edit)
endif(USE_LIBEDIT)

target_include_directories(beshfuck
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
