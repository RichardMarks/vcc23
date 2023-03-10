
set(VCC23_SOURCES
        AddInstruction.cpp
        AndInstruction.cpp
        AssignInstruction.cpp
        CodeGenerator.cpp
        CodeGenerator.h
        CompareInstruction.cpp
        DivideInstruction.cpp
        EndInstruction.cpp
        Instructions.h
        InstructionSyntax.cpp
        InstructionSyntax.h
        JumpInstruction.cpp
        LeftShiftInstruction.cpp
        LexemeType.h
        Lexeme.h
        Lexer.cpp
        Lexer.h
        MemorySelectInstruction.cpp
        MultiplyInstruction.cpp
        NegateInstruction.cpp
        NopInstruction.cpp
        OrInstruction.cpp
        Parser.cpp
        Parser.h
        ParseUtils.cpp
        ParseUtils.h
        ParserErrors.h
        PrintInstruction.cpp
        ReadInputDeviceInstruction.cpp
        RightShiftInstruction.cpp
        SubtractInstruction.cpp
        SyntaxPatterns.h
        SyntaxTree.cpp
        SyntaxTree.h
        VCCReader.cpp
        vcc23Main.cpp
        WriteOutputDeviceInstruction.cpp
        XorInstruction.cpp
        ZCompareInstruction.cpp
        )

add_executable(vcc23 ${VCC23_SOURCES})
target_link_libraries(vcc23 vc23common)
target_include_directories(vcc23 PUBLIC ../common)
set_target_properties(vcc23 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})