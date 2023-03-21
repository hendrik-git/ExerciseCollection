# -------------------------------------------------------------------------------------------------
# GameCollection Project
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)
project(
    ExerciseCollection
    VERSION 0.1.0
    DESCRIPTION "A collection of exercises"
    LANGUAGES CXX
)

# only works for Makefile Generators and Ninja Generators
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# tell cmake to look into the local /cmake subdirectory to look for modules (.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(git_commands) # update submodules, get the latest git commit
include(macros) # load custom functions and macros

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF) # to build and link dynamic or static

# set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Collect compiler flags in a variable to reuse for the following targets. Setting the compiler
# flags globally will also cause them to be used for dependencies. This may result in the
# dependencies breaking the build with warnings (with /WX or -Werror)
if(MSVC)
    set(ADDITIONAL_COMPILER_FLAGS /MP /W4 ) # /utf-8  /WX /fastfail 
else()
    #set(ADDITIONAL_COMPILER_FLAGS -Werror -Wall)
endif()

mark_as_advanced(ADDITIONAL_COMPILER_FLAGS)

# -------------------------------------------------------------------------------------------------
# Download and enable all dependencies
# -------------------------------------------------------------------------------------------------
# package manager build on top of fetch_content, see https://github.com/cpm-cmake/CPM.cmake
include(CPM) 

# replaced by std::format
CPMAddPackage("gh:fmtlib/fmt#a33701196adfad74917046096bf5a2aa0ab0bb50") # v9.1.0 [2022.08.27]
setfolderinvs(fmt Dependencies)

CPMAddPackage("gh:catchorg/Catch2#97c48e0c343d26d50764fafdc90b1e630fbd10ce") # v3.1.0 [2022.10.15]
setfolderinvs(Catch2 Dependencies)
setfolderinvs(Catch2WithMain Dependencies)

# -------------------------------------------------------------------------------------------------
# Building the targets
# -------------------------------------------------------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()    
    include(CTest)
    add_subdirectory(src)
endif()

# -------------------------------------------------------------------------------------------------
# Tools
# -------------------------------------------------------------------------------------------------
include(graphviz)
include(cmake-format)
include(cmake-lint)
include(cpp_check)
#include(poxy)
include(documentation)

# -------------------------------------------------------------------------------------------------
# Visual Studio settings
# -------------------------------------------------------------------------------------------------
# set the active project to the executable in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TestApp)
# set the working directory to the /bin directory
set_target_properties(TestApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
# neatly put all targets in their folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)