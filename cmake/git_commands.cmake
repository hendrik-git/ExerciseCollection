# -------------------------------------------------------------------------------------------------
# git commands
# -------------------------------------------------------------------------------------------------
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # ---------------------------------------------------------------------------------------------
    # Update submodules as needed
    # ---------------------------------------------------------------------------------------------
    option(GIT_UPDATE_SUBMODULE "Update submodules during build" ON)
    if(GIT_UPDATE_SUBMODULE)
        message(CHECK_START "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(CHECK_FAIL "failed")
            message(
                FATAL_ERROR
                    "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT},\\n"
                    "please checkout submodules"
            )
        endif()
        message(CHECK_PASS "successfull")
    endif()

    # ---------------------------------------------------------------------------------------------
    # Save the current commit ID as a variable
    # ---------------------------------------------------------------------------------------------
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE PACKAGE_GIT_VERSION
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Current commit id is ${PACKAGE_GIT_VERSION}")
else()
    message(WARNING "git not found - skipping git commands")
endif()
