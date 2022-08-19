
set(DIR_SRCS
        "hello_world_main.c")

idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS "include")

spiffs_create_partition_image(www ../html FLASH_IN_PROJECT)