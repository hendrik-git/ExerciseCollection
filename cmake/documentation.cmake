# -------------------------------------------------------------------------------------------------
# Doxygen
# -------------------------------------------------------------------------------------------------
# https://cmake.org/cmake/help/v3.9/module/FindDoxygen.html
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_GENERATE_HTML NO)
Doxygen_add_docs(
    Doxygen
    ${CMAKE_SOURCE_DIR}/src
    COMMENT "Create HTML documentation"
)
set_target_properties(Doxygen PROPERTIES FOLDER Tools)


# -------------------------------------------------------------------------------------------------
# Sphinx
# -------------------------------------------------------------------------------------------------
find_program(EXE_SPHINX "sphinx-build")
if(NOT EXE_SPHINX)
    message(STATUS "Sphinx executable not found! ${EXE_SPHINX}")
    message(STATUS "-- Download with pip install -U sphinx") 
    message(STATUS "-- Download with pip install sphinx_rtd_theme")
    message(STATUS "-- Download with pip install breathe")
else()
    message(STATUS "Sphinx executable found! Adding target.")

    set(SPHINX_SOURCE ${CMAKE_SOURCE_DIR}/docs/source)
    set(SPHINX_BUILD  ${CMAKE_SOURCE_DIR}/docs/build)
    set(SPHINX_DATA   ${CMAKE_SOURCE_DIR}/docs/xml)

    add_custom_target(Sphinx ALL
    COMMAND ${EXE_SPHINX} -b html
    # Tell Breathe where to find the Doxygen output
    -Dbreathe_projects.GameCollection=${SPHINX_DATA}
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Generating documentation with Sphinx")
    set_target_properties(Sphinx PROPERTIES FOLDER Tools)

endif()