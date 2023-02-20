# This macro wraps a security check around the properties folder command, so that removing a target
# does cause cmake to error trying to set a property (e.g. unchecking a part of the SFML library)
macro(SetFolderInVS targetName folderName)
    if(TARGET ${targetName})
        set_target_properties(${targetName} PROPERTIES FOLDER ${folderName})
    endif()
endmacro(SetFolderInVS)
