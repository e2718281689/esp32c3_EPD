
# set(DIR_SRCS 
#         "EPD.c")

# idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS ".")
# register_component()

idf_component_register(
    SRC_DIRS
        "." 
    INCLUDE_DIRS
        "include" )