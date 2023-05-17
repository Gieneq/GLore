message(STATUS " * Building libs")

add_library(cJSON 
    cJSON/cJSON.c
    cJSON/cJSON_Utils.c
)

target_include_directories(cJSON INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cJSON)