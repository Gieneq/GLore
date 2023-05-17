message(STATUS " * Building quest")


list(APPEND LIB_DEPENDENCIES 
    object
    data
)

message(STATUS "   -> ${LIB_DEPENDENCIES}")

add_library(quest 
  quest.c
  questlog.c
)

target_link_libraries(quest PUBLIC ${LIB_DEPENDENCIES})
target_include_directories(quest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})