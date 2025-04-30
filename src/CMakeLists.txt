CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

set(CXX_STANDARD 17)
set(C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

find_package(cpr REQUIRED)
if (cpr_FOUND)
    message(STATUS "Found cpr: ${cpr_CONFIG} (found version ${cpr_VERSION})")
endif ()

find_package(nlohmann_json REQUIRED)

add_library(gerrit_helper gerrit_helper.cpp utils.cpp)
target_include_directories(gerrit_helper PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(gerrit_helper PRIVATE cpr::cpr nlohmann_json::nlohmann_json)