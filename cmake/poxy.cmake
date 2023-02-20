# -------------------------------------------------------------------------------------------------
# poxy - document generator
# -------------------------------------------------------------------------------------------------
find_program(EXE_POXY "poxy")
if(NOT EXE_POXY)
    message(STATUS "poxy executable not found! ${EXE_POXY}")
    message(STATUS "-- Download with pip install poxy")
    message(STATUS "-- requires Python version >=3.9")
    message(STATUS "-- add Python scripts directory to path")
    message(STATUS "-- disable app execution aliases for windows")
else()
    message(STATUS "poxy executable found! Adding target.")

    add_custom_target(
        poxy ALL
        COMMAND poxy --xml
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/docs"
        COMMENT "Generating documentation with poxy"
    )
    set_target_properties(poxy PROPERTIES FOLDER Tools)
endif()
