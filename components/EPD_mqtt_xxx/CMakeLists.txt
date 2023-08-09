
# set(DIR_SRCS 
#         "EPD_mqtt.c"
#         )

# idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS "." REQUIRES mqtt freertos spi_flash nvs_flash)
# register_component()
idf_component_register(
    SRCS
        "EPD_mqtt.c" 
    INCLUDE_DIRS
        "include"
    REQUIRES 
        nvs_flash freertos
    PRIV_REQUIRES 
        nvs_flash mqtt freertos)

