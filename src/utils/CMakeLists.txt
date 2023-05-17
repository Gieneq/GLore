message(STATUS " * Building utils")

add_library(utils 
  utils.c
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})