message(STATUS " * Building dialog")

list(APPEND LIB_DEPENDENCIES 
  utils
  cJSON
  quest
  entity
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(dialog 
  dialog.c
  keywords.c
  responses.c
)

target_link_libraries(dialog PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(dialog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})