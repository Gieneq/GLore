cmake_minimum_required(VERSION 3.0.0)
project(Pyrograf VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Pyrograf 
main.c 
core.c
entity.c
)


add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
