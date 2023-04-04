# -------------------------------------------------------------------------------------------------
# External dependencies
# -------------------------------------------------------------------------------------------------

# TOML
add_library(tomlplusplus INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include/tomlplusplus/toml.hpp
)
add_library(tomlplusplus::tomlplusplus ALIAS tomlplusplus)

target_include_directories(tomlplusplus INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/tomlplusplus/include
)
setfolderinvs(tomlplusplus Dependencies)

# JSON
add_library(nlohmann INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/include/nlohmann/json.hpp
)
add_library(nlohmann::json ALIAS nlohmann)

target_include_directories(nlohmann INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/include
)

setfolderinvs(nlohmann Dependencies)