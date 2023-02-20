# The graph name. Default: value of CMAKE_PROJECT_NAME set(GRAPHVIZ_GRAPH_NAME
# ${CMAKE_PROJECT_NAME})

# The header written at the top of the Graphviz files. 
# Default: "node [ fontsize = "12" ];"
set(GRAPHVIZ_GRAPH_HEADER
    "node [ fontname=\"DejaVu Sans Mono\"; fontsize = \"18\" ];"
)

# The prefix for each node in the Graphviz files. Default: "node"
# set(GRAPHVIZ_NODE_PREFIX node)

# Set to FALSE to exclude executables from the generated graphs.
# set(GRAPHVIZ_EXECUTABLES TRUE)

# Set to FALSE to exclude static libraries from the generated graphs.
# set(GRAPHVIZ_STATIC_LIBS TRUE)

# Set to FALSE to exclude shared libraries from the generated graphs.
# set(GRAPHVIZ_SHARED_LIBS TRUE)

# Set to FALSE to exclude module libraries from the generated graphs.
# set(GRAPHVIZ_MODULE_LIBS TRUE)

# Set to FALSE to exclude interface libraries from the generated graphs.
# set(GRAPHVIZ_INTERFACE_LIBS TRUE)

# Set to FALSE to exclude object libraries from the generated graphs.
# set(GRAPHVIZ_OBJECT_LIBS TRUE)

# Set to FALSE to exclude unknown libraries from the generated graphs.
set(GRAPHVIZ_UNKNOWN_LIBS FALSE)

# Set to FALSE to exclude external libraries from the generated graphs.
# set(GRAPHVIZ_EXTERNAL_LIBS TRUE)

# Set to TRUE to include custom targets in the generated graphs.
set(GRAPHVIZ_CUSTOM_TARGETS TRUE)

# A list of regular expressions for names of targets to exclude from the
# generated graphs.
set(GRAPHVIZ_IGNORE_TARGETS 
    "${CMAKE_BINARY_DIR}/_deps/*" 
    "CLI11_warnings" 
    "fmt-header-only" 
    "sfml-main"
    "legacy_stdio_definitions.lib"
    "winmm"
    "ws2_32"
    "ftxui_*"
)

# Set to FALSE to not generate per-target graphs foo.dot.<target>.
set(GRAPHVIZ_GENERATE_PER_TARGET FALSE)

# Set to FALSE to not generate depender graphs foo.dot.<target>.dependers.
set(GRAPHVIZ_GENERATE_DEPENDERS FALSE)
