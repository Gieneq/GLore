message(STATUS " * Building core")

list(APPEND LIB_DEPENDENCIES 
  utils
  entity
  world
  data
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(core 
  core.c
  system_user_input.c
  system_dialogs.c
  system_player.c
  system_help.c
  system_debug_message.c
  system_quest.c
)

target_link_libraries(core PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})