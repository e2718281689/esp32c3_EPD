
set(DIR_SRCS
       "EPD_console.c"
)
idf_component_register(SRCS ${DIR_SRCS}
       INCLUDE_DIRS "."
       REQUIRES console )
