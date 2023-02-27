
set(VC23_SOURCES
        ConsoleRuntime.cpp
        ConsoleRuntime.h
        Runtime.cpp
        Runtime.h
        vc23Main.cpp
        )

add_executable(vc23 ${VC23_SOURCES})
target_link_libraries(vc23 vc23common)
target_include_directories(vc23 PUBLIC ../common)
set_target_properties(vc23 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})