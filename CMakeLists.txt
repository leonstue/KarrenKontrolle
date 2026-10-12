
cmake_minimum_required(VERSION 3.21)

project(karrenkontrolle VERSION 0.1.0 LANGUAGES CXX)

# Set which C++ version to use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Drogon CONFIG REQUIRED)
find_package(reflectcpp CONFIG REQUIRED)

# Create executable
add_executable(karrenkontrolle src/main.cpp)

# Source files
target_sources(karrenkontrolle PRIVATE
    src/server/src/Server.cpp
    src/server/routes/src/Routes.cpp
    src/infrastructure/logging/src/Logging.cpp
    src/config/src/AppConfig.cpp
    src/server/middleware/src/AuthGuard.cpp
)

# Find all header files recursively
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
)

# Collect all the header directories
set(HEADER_DIRS "")

foreach(HEADER_FILE IN LISTS HEADER_FILES)
    get_filename_component(HEADER_DIR "${HEADER_FILE}" DIRECTORY)
    list(APPEND HEADER_DIRS "${HEADER_DIR}")
endforeach()

list(REMOVE_DUPLICATES HEADER_DIRS)

# Include header directories
target_include_directories(karrenkontrolle PRIVATE
    ${HEADER_DIRS}
)

# Link dependencies
target_link_libraries(karrenkontrolle PRIVATE
    Drogon::Drogon
    reflectcpp::reflectcpp
)
