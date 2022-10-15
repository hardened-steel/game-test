cmake_minimum_required(VERSION 3.16)
project(game LANGUAGES CXX C VERSION 0.0.1)

include(FetchContent)
enable_testing()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

set(SOURCES
    map.cpp
)

add_library(${PROJECT_NAME}-lib ${SOURCES})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
install(TARGETS ${PROJECT_NAME}-lib)

add_executable(${PROJECT_NAME}
    main.cpp
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS YES
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-lib)
#add_test(NAME LibUnits COMMAND ${PROJECT_NAME}-test)
#install(TARGETS ${PROJECT_NAME}-test RUNTIME DESTINATION bin)

#sinclude(CPack)
