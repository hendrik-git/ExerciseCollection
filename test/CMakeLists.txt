# -------------------------------------------------------------------------------------------------
# Setup
# -------------------------------------------------------------------------------------------------
include(Catch)

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameEngine library
# -------------------------------------------------------------------------------------------------
add_executable(TestApp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataSerialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DesignPatternProblems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LanguageFeatures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MathProblems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringProblems.cpp
)
target_link_libraries(TestApp PRIVATE Catch2::Catch2WithMain ExerciseCollection)
set_target_properties(TestApp PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
)
target_compile_features(TestApp PUBLIC cxx_std_20)
#SetFolderInVS(TestApp Test)

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET TestApp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:TestApp>
                $<TARGET_FILE_DIR:TestApp>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

#add_test(NAME GameEngine COMMAND TestApp)
catch_discover_tests(TestApp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)

# -------------------------------------------------------------------------------------------------
# Code coverage
# -------------------------------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable coverage" ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND ENABLE_COVERAGE)
    include(code_coverage)
    message(STATUS "Enabled code coverage with clang")

    target_compile_options(TestApp PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)
    target_link_libraries(TestApp PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)

    generate_coverage_report_for(TestApp)
    #generate_coverage_report_for(Test_GameMath)
endif()