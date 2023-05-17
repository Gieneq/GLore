message(STATUS " * Building world")

list(APPEND LIB_DEPENDENCIES 
  utils
  entity
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(world 
  room.c
  world.c
)

target_link_libraries(world PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(world INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})