project(game-engine-lib LANGUAGES CXX C VERSION 0.0.1)

set(SOURCES
    map.cpp
    tick.event.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../)
install(TARGETS ${PROJECT_NAME})
