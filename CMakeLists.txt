cmake_minimum_required(VERSION 3.0.0)
project(Pyrograf VERSION 0.1.0)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include(CTest)
enable_testing()


include_directories(lib/cJSON)

add_executable(Pyrograf 
main.c 
core.c
entity.c
item.c
loader.c
database.c
world.c
lib/cJSON/cJSON.c
lib/cJSON/cJSON_Utils.c
)



add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
