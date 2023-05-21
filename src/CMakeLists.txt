message(STATUS "Building src")


# Refresh asset
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res/)
file(COPY ../res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(utils)
add_subdirectory(quest)
add_subdirectory(object)
add_subdirectory(data)
add_subdirectory(world)
add_subdirectory(core)
add_subdirectory(dialog)
add_subdirectory(entity)

list(APPEND DEP_LIBS
    cJSON
    utils
    quest
    object
    data
    core
    entity
    dialog
    world
)

add_executable(GLore
    main.c
)

target_link_libraries(GLore PUBLIC ${DEP_LIBS})

target_include_directories(GLore PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

