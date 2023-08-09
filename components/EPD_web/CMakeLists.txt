
# set(DIR_SRCS 
#         "web_serve.c")

# idf_component_register(SRCS ${DIR_SRCS} INCLUDE_DIRS ".")
# register_component()

idf_component_register(
    SRCS
        "web_serve.c" 
    INCLUDE_DIRS
        "include" 
    REQUIRES 
        
    PRIV_REQUIRES 
        display nvs_flash esp_http_server
        )