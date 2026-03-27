project(ws_core)

# Set sources
file(
    GLOB_RECURSE ${PROJECT_NAME}_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Create executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

# Target includes
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Link all libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ZLIB::ZLIB
    OpenSSL::SSL
    Crow::Crow
)

# Set warning options
set(OPTIONS_WARNINGS -Wall )#-Wpedantic)

# Set build type optimisations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OPTIONS_OPTIMIZATION -O2)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIONS_OPTIMIZATION -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(OPTIONS_OPTIMIZATION -O2 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(OPTIONS_OPTIMIZATION -Os)
endif()

# Print some information for debugging
message(STATUS "Build type:" ${CMAKE_BUILD_TYPE})
message(STATUS "Compiler options: ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION}")

# Set compile options
target_compile_options(${PROJECT_NAME} PRIVATE ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION})

# Set precompile headers
target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    <string>
    <string_view>
    <format>
    <regex>
    <optional>
    <utility>
    <functional>
    <memory>
    <mutex>
    <exception>
    <vector>
    <unordered_map>
    <algorithm>
    <chrono>
)