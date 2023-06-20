message(STATUS " * Building object")

list(APPEND LIB_DEPENDENCIES 
  utils
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(object 
  item.c
  item_database.c
  container.c
)

target_link_libraries(object PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(object INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})