# #file(GLOB  DIR_SRCS  "./*.c")
# #file(GLOB  DIR_SRCS  "./*.cpp")

# set(DIR_SRCS
#         "FATFS_sd.c"
#         "SPIFFS_flash.c"
#         )

# idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS ".")
# register_component()

idf_component_register(
    SRCS
        "FATFS_sd.c" 
        "SPIFFS_flash.c"
    INCLUDE_DIRS
        "include" 
    REQUIRES 
        vfs
    PRIV_REQUIRES 
        freertos sdmmc spiffs fatfs 
        )