message(STATUS " * Building data")

list(APPEND LIB_DEPENDENCIES 
  utils
  object
  quest
  world
  core
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(data 
  loader.c
)

target_link_libraries(data PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(data INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})