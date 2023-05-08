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
    set(ADDITIONAL_COMPILER_FLAGS /MP /W4 /diagnostics:caret /fastfail /utf-8 ) # /WX  
else()
    #set(ADDITIONAL_COMPILER_FLAGS -Werror -Wall)
endif()

mark_as_advanced(ADDITIONAL_COMPILER_FLAGS)

# -------------------------------------------------------------------------------------------------
# Download and enable all dependencies
# -------------------------------------------------------------------------------------------------
# package manager build on top of fetch_content, see https://github.com/cpm-cmake/CPM.cmake
include(CPM)
# CPMAddPackage("uri@version")     # A git package from a given uri with a version
# CPMAddPackage("uri#tag")         # A git package from a given uri with a git tag or commit hash
# CPMAddPackage("uri@version#tag") # A git package with both version and tag provided

# replaced by std::format
CPMAddPackage("gh:fmtlib/fmt#a33701196adfad74917046096bf5a2aa0ab0bb50") # v9.1.0 [2022.08.27]
setfolderinvs(fmt Dependencies)

CPMAddPackage("gh:catchorg/Catch2#97c48e0c343d26d50764fafdc90b1e630fbd10ce") # v3.1.0 [2022.10.15]
setfolderinvs(Catch2 Dependencies)
setfolderinvs(Catch2WithMain Dependencies)

CPMAddPackage("gh:zeux/pugixml@1.13")
setfolderinvs(pugixml-static Dependencies)

CPMAddPackage("gh:abdes/cryptopp-cmake#CRYPTOPP_8_7_0_1")
setfolderinvs(cryptopp Dependencies)

CPMAddPackage(
  NAME Boost
  VERSION 1.82.0
  GITHUB_REPOSITORY "boostorg/boost"
  GIT_TAG "boost-1.82.0"
)
setfolderinvs(boost_assert Dependencies/Boost)
setfolderinvs(boost_atomic Dependencies/Boost)
setfolderinvs(boost_chrono Dependencies/Boost)
setfolderinvs(boost_container Dependencies/Boost)
setfolderinvs(boost_context Dependencies/Boost)
setfolderinvs(boost_contract Dependencies/Boost)
setfolderinvs(boost_core Dependencies/Boost)
setfolderinvs(boost_coroutine Dependencies/Boost)
setfolderinvs(boost_date_time Dependencies/Boost)
setfolderinvs(boost_exception Dependencies/Boost)
setfolderinvs(boost_fiber Dependencies/Boost)
setfolderinvs(boost_fiber_numa Dependencies/Boost)
setfolderinvs(boost_filesystem Dependencies/Boost)
setfolderinvs(boost_graph Dependencies/Boost)
setfolderinvs(boost_iostreams Dependencies/Boost)
setfolderinvs(boost_json Dependencies/Boost)
setfolderinvs(boost_locale Dependencies/Boost)
setfolderinvs(boost_log Dependencies/Boost)
setfolderinvs(boost_log_setup Dependencies/Boost)
setfolderinvs(boost_nowide Dependencies/Boost)
setfolderinvs(boost_prg_exec_monitor Dependencies/Boost)
setfolderinvs(boost_program_options Dependencies/Boost)
setfolderinvs(boost_random Dependencies/Boost)
setfolderinvs(boost_serialization Dependencies/Boost)
setfolderinvs(boost_stacktrace_basic Dependencies/Boost)
setfolderinvs(boost_stacktrace_noop Dependencies/Boost)
setfolderinvs(boost_stacktrace_windbg Dependencies/Boost)
setfolderinvs(boost_stacktrace_windbg_cached Dependencies/Boost)
setfolderinvs(boost_system Dependencies/Boost)
setfolderinvs(boost_test_exec_monitor Dependencies/Boost)
setfolderinvs(boost_thread Dependencies/Boost)
setfolderinvs(boost_timer Dependencies/Boost)
setfolderinvs(boost_type_erasure Dependencies/Boost)
setfolderinvs(boost_unit_test_framework Dependencies/Boost)
setfolderinvs(boost_url Dependencies/Boost)
setfolderinvs(boost_variant2 Dependencies/Boost)
setfolderinvs(boost_wave Dependencies/Boost)
setfolderinvs(boost_wserialization Dependencies/Boost)

# -------------------------------------------------------------------------------------------------
# Building the targets
# -------------------------------------------------------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()    
    include(CTest)
    add_subdirectory(extern)
    add_subdirectory(src)
    add_subdirectory(test)
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