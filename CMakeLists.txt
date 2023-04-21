cmake_minimum_required(VERSION 3.0.0)
project(Pyrograf VERSION 0.1.0)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include(CTest)
enable_testing()


include_directories(lib/cJSON)
include_directories(src/core)
include_directories(src/entity)
include_directories(src/objects)
include_directories(src/quests)
include_directories(src/utils)

add_executable(Pyrograf 
main.c 
src/core/core.c
src/core/loader.c
src/core/database.c
src/entity/player.c
src/objects/item.c
src/objects/world.c
src/quests/quest.c

lib/cJSON/cJSON.c
lib/cJSON/cJSON_Utils.c
)



add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
