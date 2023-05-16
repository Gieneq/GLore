cmake_minimum_required(VERSION 3.6)
project(GLore VERSION 0.0.3)
set(CMAKE_CXX_STANDARD 14)

#############################
#           TESTS           #
#############################
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(GLore_test
test/utils_test.cpp
)

target_link_libraries(
    GLore_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(GLore_test)

#############################
#          PROJECT          #
#############################

include_directories(lib/cJSON)
include_directories(src/core)
include_directories(src/dialogs)
include_directories(src/entity)
include_directories(src/objects)
include_directories(src/quests)
include_directories(src/utils)

add_executable(GLore 
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
