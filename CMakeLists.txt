CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(Gerrit_helper)

set(CXX_STANDARD 17)
set(C_STANDARD 99)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE gerrit_helper)
