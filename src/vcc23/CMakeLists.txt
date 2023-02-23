
set(VCC23_SOURCES
        CodeGenerator.cpp
        CodeGenerator.h
        Lexer.cpp
        Lexer.h
        Parser.cpp
        Parser.h
        ParserErrors.h
        SyntaxTree.cpp
        SyntaxTree.h
        VCCReader.cpp
        main.cpp
        )

add_executable(vcc23 ${VCC23_SOURCES})
target_link_libraries(vcc23 vc23common)
target_include_directories(vcc23 PUBLIC ../common)
set_target_properties(vcc23 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})