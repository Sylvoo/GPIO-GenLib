cmake_minimum_required(VERSION 3.20)

project(GPIO_Lib_Project
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
)

find_package(doctest REQUIRED)

add_executable(${PROJECT_NAME}_tests
    tests/test_main.cpp
)
target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
)

target_compile_options(${PROJECT_NAME}_tests
    PRIVATE
        -Wall
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE doctest::doctest)




