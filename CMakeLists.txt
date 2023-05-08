cmake_minimum_required(VERSION 3.0.0)
project(Pyrograf VERSION 0.1.0)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include(CTest)
enable_testing()


include_directories(lib/cJSON)
include_directories(src/core)
include_directories(src/dialogs)
include_directories(src/entity)
include_directories(src/objects)
include_directories(src/quests)
include_directories(src/utils)

add_executable(Pyrograf 
main.c 
src/core/core.c
src/core/loader.c
src/core/loader_testyard.c
src/core/database.c
src/core/system_user_input.c
src/dialogs/keywords.c
src/dialogs/dialog.c
src/dialogs/responses.c
src/entity/player.c
src/entity/npc.c
src/objects/item.c
src/objects/world.c
src/objects/room.c
src/quests/quest.c
src/quests/questlog.c
src/utils/utils.c

lib/cJSON/cJSON.c
lib/cJSON/cJSON_Utils.c
)



add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
