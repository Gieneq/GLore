message(STATUS " * Building entity")

list(APPEND LIB_DEPENDENCIES 
  utils
  quest
  world
  dialog
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(entity 
  npc.c
  player.c
)

target_link_libraries(entity PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(entity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})