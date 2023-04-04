# -------------------------------------------------------------------------------------------------
# External dependencies
# -------------------------------------------------------------------------------------------------
add_library(tomlplusplus INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include/tomlplusplus/toml.hpp
)
add_library(tomlplusplus::tomlplusplus ALIAS tomlplusplus)

target_include_directories(tomlplusplus INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include
)

setfolderinvs(tomlplusplus Dependencies)