# -------------------------------------------------------------------------------------------------
# ExerciseCollection Library
# -------------------------------------------------------------------------------------------------
add_library(ExerciseCollection STATIC
    # Header files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/Cryptography.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/DataSerialization.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/DesignPatternProblems.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/LanguageFeatures.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/Logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/MathProblems.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ExerciseCollection/StringProblems.hpp
    # Source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Cryptography.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataSerialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DesignPatternProblems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LanguageFeatures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MathProblems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringProblems.cpp
)
add_library(Exercise::Collection ALIAS ExerciseCollection)

target_link_libraries(ExerciseCollection 
    PUBLIC
        fmt::fmt
    PRIVATE
        tomlplusplus::tomlplusplus
        nlohmann::json
        pugixml::pugixml
        cryptopp::cryptopp
)
target_include_directories(ExerciseCollection 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(ExerciseCollection PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
)
target_compile_features(ExerciseCollection PUBLIC cxx_std_20)
#SetFolderInVS(ExerciseCollection Test)

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET ExerciseCollection
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ExerciseCollection>
                $<TARGET_FILE_DIR:ExerciseCollection>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

# -------------------------------------------------------------------------------------------------
# Code coverage
# -------------------------------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable coverage" ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND ENABLE_COVERAGE)
    include(code_coverage)
    message(STATUS "Enabled code coverage with clang")

    target_compile_options(ExerciseCollection PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)
    target_link_libraries(ExerciseCollection PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)

    generate_coverage_report_for(ExerciseCollection)
endif()