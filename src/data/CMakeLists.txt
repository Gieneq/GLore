message(STATUS " * Building data")

list(APPEND LIB_DEPENDENCIES 
  utils
  object
  quest
  world
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(data 
  database.c
  loader.c
)

target_link_libraries(data PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(data INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})