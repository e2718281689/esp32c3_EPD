file(GLOB  DIR_SRCS  "./*.c")

idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS "")
#spiffs_create_partition_image(www ../html FLASH_IN_PROJECT)
