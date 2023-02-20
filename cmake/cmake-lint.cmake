# -------------------------------------------------------------------------------------------------
# cmake lintter
# -------------------------------------------------------------------------------------------------
find_program(EXE_CMAKELINT "cmake-lint")
if(NOT EXE_CMAKELINT)
    message(STATUS "cmake-lint executable not found! ${EXE_CMAKELINT}")
    message(STATUS "-- Download with pip install cmakelang")
else()
    message(STATUS "cmake-lint executable found! Adding target.")

    # find all cmake files to be linted
    file(GLOB_RECURSE CMAKE_FILE_LIST cmake/*.cmake)
    list(APPEND CMAKE_FILE_LIST ${CMAKE_SOURCE_DIR}/CMakeLists.txt)

    add_custom_target(
        cmake-lint ALL
        COMMAND cmake-lint -c ${CMAKE_SOURCE_DIR}/tools/cmakelang.py -- ${CMAKE_FILE_LIST}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Linting CMake files"
    )
    set_target_properties(cmake-lint PROPERTIES FOLDER Tools)
endif()
