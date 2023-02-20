function(generate_coverage_report_for TARGET_NAME)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND ENABLE_COVERAGE)

add_custom_target(${TARGET_NAME}-ccov-preprocessing
    #COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}>
    #COMMAND llvm-profdata merge -sparse ${TARGET_NAME}.profraw -o ${TARGET_NAME}.profdata
    COMMAND $<TARGET_FILE:${TARGET_NAME}>
    COMMAND llvm-profdata merge default.profraw -o ${TARGET_NAME}.profdata # -sparse
    DEPENDS ${TARGET_NAME}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
    )

#add_custom_target(${TARGET_NAME}-ccov-show
#    COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions
#    DEPENDS ${TARGET_NAME}-ccov-preprocessing
#    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
#    )

#add_custom_target(${TARGET_NAME}-ccov-report
#    COMMAND llvm-cov report $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata
#    DEPENDS ${TARGET_NAME}-ccov-preprocessing
#    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
#    )

add_custom_target(${TARGET_NAME}-ccov
    COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions -output-dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}-llvm-cov -format="html" -ignore-filename-regex=build*
    DEPENDS ${TARGET_NAME}-ccov-preprocessing
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
    )

add_custom_command(TARGET ${TARGET_NAME}-ccov POST_BUILD
    COMMAND ;
    COMMENT "Open ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}-llvm-cov/index.html in your browser to view the coverage report."
)

endif()
endfunction(generate_coverage_report_for)