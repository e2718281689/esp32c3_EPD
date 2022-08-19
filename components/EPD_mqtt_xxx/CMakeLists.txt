
set(DIR_SRCS 
        "EPD_mqtt.c"
        )

idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS "." REQUIRES mqtt freertos )