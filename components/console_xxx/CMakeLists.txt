
#set(DIR_SRCS
#       "EPD_console.c"
#)
#idf_component_register(SRCS ${DIR_SRCS}
#       INCLUDE_DIRS "."
#       REQUIRES console )

# set(COMPONENT_SRCDIRS "EPD_console.c")
# set(COMPONENT_ADD_INCLUDEDIRS "include")

# set(COMPONENT_REQUIRES console)

# register_component()

idf_component_register(
    SRC_DIRS
        "." 
    INCLUDE_DIRS
        "include" 
    REQUIRES 
        vfs
    PRIV_REQUIRES 
        console  driver)