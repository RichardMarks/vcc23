cmake_minimum_required(VERSION 3.21)
project(vcc23 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(VCC23_SOURCES
        src/main.cpp
        src/VCCReader.cpp
        src/CommandLine.cpp src/Lexer.cpp src/Lexer.h src/Parser.cpp src/Parser.h src/Syntax.h src/SyntaxTree.cpp src/SyntaxTree.h src/ParserErrors.h)

add_executable(vcc23 ${VCC23_SOURCES})

target_include_directories(vcc23 PRIVATE src)
