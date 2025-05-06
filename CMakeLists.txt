CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(Gerrit_helper)

set(CXX_STANDARD 17)
set(C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:/usr/local/include/gerrit_helper>
    )
target_link_directories(${PROJECT_NAME} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
    )
target_link_libraries(${PROJECT_NAME} PRIVATE gerrit_helper)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "/usr/local/lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
install(TARGETS gerrit_helper LIBRARY DESTINATION /usr/local/lib)
install(DIRECTORY include/ DESTINATION /usr/local/include/gerrit_helper FILES_MATCHING PATTERN "*.hpp")
