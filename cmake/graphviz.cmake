# -------------------------------------------------------------------------------------------------
# graphviz - dependency visualization
# -------------------------------------------------------------------------------------------------
find_program(EXE_DOT "dot")
if(NOT EXE_DOT)
    message(STATUS "Graphviz executable not found! ${EXE_DOT}")
    message(STATUS "-- Download from https://graphviz.org/download/")
    message(STATUS "-- or install with sudo apt install graphviz")
else()
    message(STATUS "Graphviz executable found! Adding target.")
    add_custom_target(
        graphviz ALL
        COMMAND ${CMAKE_COMMAND} "--graphviz=GameCollectionApp.dot" .
        COMMAND dot -Tpng GameCollectionApp.dot -o ${CMAKE_SOURCE_DIR}/docs/GameCollectionApp.png
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Generating dependency graph in docs/"
    )
    set_target_properties(graphviz PROPERTIES FOLDER Tools)
endif()
