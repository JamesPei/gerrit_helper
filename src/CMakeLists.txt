CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

set(CXX_STANDARD 17)
set(C_STANDARD 99)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_library(gerrit_helper gerrit_helper.cpp)
target_include_directories(gerrit_helper PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(gerrit_helper PRIVATE cpr::cpr)