message(STATUS "Building src")

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
