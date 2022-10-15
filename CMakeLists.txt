cmake_minimum_required(VERSION 3.16)
project(warriors LANGUAGES CXX C VERSION 0.0.1)

add_subdirectory(game)

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
target_link_libraries(${PROJECT_NAME} PUBLIC game-commands-lib)
#add_test(NAME LibUnits COMMAND ${PROJECT_NAME}-test)
#install(TARGETS ${PROJECT_NAME}-test RUNTIME DESTINATION bin)

#sinclude(CPack)
